// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BP_Monster.generated.h"

UCLASS()
class FPS_API ABP_Monster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABP_Monster();

	UPROPERTY(BlueprintReadWrite)
		float HP = 100;

	UPROPERTY(BlueprintReadWrite)
		float MaxHp = 100;

	UPROPERTY(BlueprintReadWrite)
		float ATK = 5;

	UPROPERTY(BlueprintReadWrite)
		float SurchDistance = 1500;

	UPROPERTY(BlueprintReadWrite)
		float ReSpawnDelay = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* DeathMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> Tag;

	UPROPERTY(BlueprintReadWrite)
	class AFPSCharacter* player;

	UPROPERTY(BlueprintReadWrite)
		bool PassibleAttack = true;

	UPROPERTY(BlueprintReadWrite)
		bool IsDeath = false;

	UPROPERTY(BlueprintReadWrite)
		bool IsStop = false;

	UPROPERTY(BlueprintReadWrite)
		bool IsSlow = false;

	FVector OrigineLocation;

	FTimerHandle DeathCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION()
		bool AddDamage(float Damage);

	UFUNCTION(BlueprintCallable)
		void Death();

	UFUNCTION(BlueprintCallable)
		void Attack();

	UFUNCTION(BlueprintCallable)
		void TimeStop();

	UFUNCTION(BlueprintCallable)
		void TimeStart();

	UFUNCTION(BlueprintCallable)
		void ResetAbility();

	void MonsterReset();
};
