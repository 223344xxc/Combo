// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "FPSMonster.h"
#include "BP_Monster.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	
	InitialLifeSpan = 3.0f;

	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	GravityScale = ProjectileMovementComponent->ProjectileGravityScale;
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (owner->IsStop == true && NowStop == false) {
		CustomTimeDilation = 0;
		NowStop = true;
	}
	else if (owner->IsStop == false && NowStop == true) {
		if(SlowStart){
			CustomTimeDilation = 0.1f;
		}
		else {
			CustomTimeDilation = 1;
		}
		NowStop = false;
	}

}

void AFPSProjectile::FireInDirection(const FVector& ShootDirection) {
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


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {

}

void  AFPSProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {

	AFPSCharacter* actor = Cast<AFPSCharacter>(GetOwner());
	if (actor != nullptr) {
		actor->SetComboCount(actor->ComboCount - 5);
	}
	Destroy();
}


void AFPSProjectile::NotifyActorBeginOverlap(AActor* OtherActor) {

	if(OtherActor->ActorHasTag(Tag[0])){
		SlowStart = true;
		CustomTimeDilation = 0.1f;
		return;
	}
	if (OtherActor->ActorHasTag(Tag[1])) {
		AFPSCharacter* actor = Cast<AFPSCharacter>(GetOwner());
		if (actor != nullptr) {
			actor->SetComboCount(actor->ComboCount + 1);
			ABP_Monster* HitMob = Cast<ABP_Monster>(OtherActor);
			if (HitMob != nullptr) {
				if (HitMob->AddDamage(actor->PlayerAD)) {
					actor->AddExp(30);
				}
			}
		}
		Destroy();
	}
}

void AFPSProjectile::NotifyActorEndOverlap(AActor* OtherActor) {

	if(OtherActor->ActorHasTag(Tag[0])){
		if (NowStop) {
			CustomTimeDilation = 0;
		}
		else {
			CustomTimeDilation = 1;
		}
		SlowStart = false;
	}
}
