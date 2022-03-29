// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

UCLASS()
class FPS_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement");
	class UProjectileMovementComponent* ProjectileMovementComponent;


	class AFPSCharacter* owner;
	FVector TempVel;

	UPROPERTY(BlueprintReadWrite)
	FVector BulletVel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GravityScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SlowScale = 0.05f;

	bool SlowStart = false;
	bool NowStop = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
