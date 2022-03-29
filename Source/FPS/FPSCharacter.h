// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FPSCharacter.generated.h"

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	FVector MuzzleOffset;

	FVector OrigineLocation;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSBoom> BoomClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MuzzlePosition;

	UPROPERTY(BlueprintReadOnly)
	class UAudioComponent* AudioComponent;

	UPROPERTY(BlueprintReadWrite)
	class USoundCue* SoundCue;

	UPROPERTY(BlueprintReadWrite)
	int ComboCount = 0;

	UPROPERTY(BlueprintReadWrite)
	int MaxComboCount = 30;

	UPROPERTY(BlueprintReadWrite)
	int Level = 1;

	UPROPERTY(BlueprintReadWrite)
	float Exp = 0;

	UPROPERTY(BlueprintReadWrite)
	float MaxExp;

	UPROPERTY(BlueprintReadWrite)
	int MaxLevel = 30;

	UPROPERTY(BlueprintReadWrite)
	float PlayerAD = 1;

	UPROPERTY(BlueprintReadWrite)
		float PlayerHp = 100;

	UPROPERTY(BlueprintReadWrite)
		float PlayerMaxHp = 100;

	UPROPERTY(BlueprintReadWrite)
	bool PlayShootSound = false;

	bool IsFire = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsStop = false;


	float runSpeed = 50;
	float FireDelay = 0.1f;
	float FireColl = 0;

	UPROPERTY(BlueprintReadWrite)
	float ComboResetDelay = 3;

	UPROPERTY(BlueprintReadWrite)
	float ComboResetTime = 0;

	FVector moveDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void TimeBoom();

	UFUNCTION()
	void TimeStop();

	UFUNCTION()
	void SetComboCount(int Combo);

	UFUNCTION(BlueprintCallable)
	void AddExp(float xp);
};
