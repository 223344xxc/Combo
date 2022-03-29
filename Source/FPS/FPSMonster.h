// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSMonster.generated.h"

UCLASS()
class FPS_API AFPSMonster : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFPSMonster();

	UPROPERTY(BlueprintReadWrite)
	float HP = 300;

	UPROPERTY(BlueprintReadWrite)
	float MaxHp = 300;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool AddDamage(float Damage);

};
