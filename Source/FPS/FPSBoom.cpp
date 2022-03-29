// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBoom.h"
#include "FPSCharacter.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
// Sets default values
AFPSBoom::AFPSBoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

	InitialLifeSpan = 3.0f;

	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSBoom::OnHit);
	ProjectileMovementComponent->ProjectileGravityScale = 3;
	GravityScale = ProjectileMovementComponent->ProjectileGravityScale;
}

// Called when the game starts or when spawned
void AFPSBoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBoom::FireInDirection(const FVector& ShootDirection) {
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;


	if (SlowStart) {
		BulletVel = GetVelocity();
		ProjectileMovementComponent->Velocity = BulletVel * SlowScale;
		ProjectileMovementComponent->ProjectileGravityScale = GravityScale * SlowScale;
		BulletVel = GetVelocity();
		ProjectileMovementComponent->Velocity = BulletVel * SlowScale;
	}
	owner = Cast<AFPSCharacter>(GetOwner());
}

// Called every frame
void AFPSBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (owner->IsStop == true && NowStop == false) {
		TempVel = ProjectileMovementComponent->Velocity;
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
		ProjectileMovementComponent->ProjectileGravityScale = 0;
		NowStop = true;
	}
	else if (owner->IsStop == false && NowStop == true) {
		ProjectileMovementComponent->Velocity = TempVel;
		ProjectileMovementComponent->ProjectileGravityScale = 1;
		NowStop = false;
	}
}

void AFPSBoom::NotifyActorBeginOverlap(AActor* OtherActor) {
	SlowStart = true;
	BulletVel = GetVelocity();
	ProjectileMovementComponent->Velocity = BulletVel * SlowScale;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale * SlowScale;
	if (OtherActor->Tags == TimeAreaTag) {
		BulletVel = GetVelocity();
		ProjectileMovementComponent->Velocity = BulletVel * SlowScale;
	}
}

void AFPSBoom::NotifyActorEndOverlap(AActor* OtherActor) {
	if (OtherActor->Tags == TimeAreaTag) {
		BulletVel = GetVelocity();
		ProjectileMovementComponent->Velocity = BulletVel * 10;
	}
	ProjectileMovementComponent->Velocity = BulletVel * 10;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale;
}

void  AFPSBoom::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	GetWorld()->SpawnActor<AActor>(SlowArea, GetActorLocation(), GetActorRotation(),SpawnParams);
	Destroy();
}

void AFPSBoom::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	/*FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	GetWorld()->SpawnActor<AActor>(SlowArea, GetActorLocation(), GetActorRotation(),SpawnParams);
	Destroy();*/
}