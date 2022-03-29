// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowArea.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

// Sets default values
ASlowArea::ASlowArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SeeingTroughMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SeeingThroughMesh"));
	RootComponent = SeeingTroughMesh;
	SeeingTroughMesh->CreateDynamicMaterialInstance(0);

	SeeingTroughMesh->SetCollisionProfileName(TEXT("InvisibleWall"));
	PrimaryActorTick.SetTickFunctionEnable(false);


}

// Called when the game starts or when spawned
void ASlowArea::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASlowArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

