// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMonster.h"

// Sets default values
AFPSMonster::AFPSMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSMonster::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AFPSMonster::AddDamage(float Damage) {
	HP -= Damage;
	if (HP <= 0) {
		Destroy();
		return true;
	}
	return false;
}