// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentTime += DeltaTime;
	if (CurrentTime >= SpawnTime) {
		CurrentTime = 0;
		SpawnMonster();
	}
}

void AMonsterSpawner::SpawnMonster() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	GetWorld()->SpawnActor<AActor>(Monster, GetActorLocation(), GetActorRotation(), SpawnParams);
}

