// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_Monster.h"
#include "FPSCharacter.h"


// Sets default values
ABP_Monster::ABP_Monster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_Monster::BeginPlay()
{
	Super::BeginPlay();
	OrigineLocation = GetActorLocation();

}

// Called every frame
void ABP_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABP_Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABP_Monster::AddDamage(float Damage) {
	if (IsDeath) {
		return false;
	}
	else {
		HP -= Damage;
		if (HP <= 0) {
			//Destroy();
			if (GetMesh()) {
				if (GetMesh()->GetAnimInstance()) {
					GetMesh()->GetAnimInstance()->Montage_Stop(0);
					GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
					IsDeath = true;
				}
			}
			return true;
		}
		return false;
	}
}


void ABP_Monster::Death() {
	//GetMesh()->GetAnimInstance()->Montage_Pause();
	GetMesh()->GetAnimInstance()->Montage_SetPlayRate(DeathMontage, 0);
	UE_LOG(LogTemp,Log, TEXT("Death"))
	GetWorldTimerManager().SetTimer(DeathCount, this, &ABP_Monster::MonsterReset, ReSpawnDelay, false);
}

void ABP_Monster::MonsterReset() {
	GetMesh()->GetAnimInstance()->Montage_SetPlayRate(DeathMontage, 1);
	GetMesh()->GetAnimInstance()->Montage_Stop(0);
	HP = MaxHp;
	SetActorLocation(OrigineLocation);
	IsDeath = false;
}

void ABP_Monster::Attack() {
	if (GetMesh() && PassibleAttack == true) {
		if (GetMesh()->GetAnimInstance()) {
			PassibleAttack = false;
			GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
		}
	}
}

void ABP_Monster::TimeStop() {
	IsStop = true;
	CustomTimeDilation = 0;
}

void ABP_Monster::TimeStart() {
	IsStop = false;
	CustomTimeDilation = 1;
}

void ABP_Monster::NotifyActorBeginOverlap(AActor* OtherActor) {
	if (OtherActor->ActorHasTag(Tag[1]) && !IsStop) {
		IsSlow = true;

		CustomTimeDilation = 0.1f;
	}
}

void ABP_Monster::NotifyActorEndOverlap(AActor* OtherActor) {
	if (OtherActor->ActorHasTag(Tag[1]) && !IsStop) {
		IsSlow = false;
		CustomTimeDilation = 1;
	}
}

void ABP_Monster::ResetAbility() {
	HP = MaxHp;
}