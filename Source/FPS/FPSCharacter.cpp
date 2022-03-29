// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "FPSBoom.h"
#include "FPSProjectile.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
		

	FPSCameraComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(GetCapsuleComponent()));

	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;
	
	ComboResetTime = ComboResetDelay;
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	MaxExp = Level * 100;
	MaxComboCount = Level * 10;
	PlayerAD = 5 + (ComboCount);
	moveDirection = FVector::ZeroVector;
	OrigineLocation = GetActorLocation();
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FireColl > 0) {
		FireColl -= DeltaTime;
		if (FireColl < 0)
			FireColl = 0;
	}

	if (IsFire && FireColl <= 0) {
		Fire();
		FireColl = FireDelay;
	}

	if (ComboCount > 0) {
		ComboResetTime -= DeltaTime * (1 + (15 * IsStop));
		if (ComboResetTime <= 0) {
			ComboResetTime = ComboResetDelay;
			ComboCount -= 1;

			if (IsStop) {
				if (ComboCount <= 0) {
					IsStop = !IsStop;
				}
			}
		}
	}

	if (PlayerHp < PlayerMaxHp) {
		PlayerHp += 1 * DeltaTime;
	}

	if (PlayerHp <= 0) {
		SetActorLocation(OrigineLocation);
		Exp = 0;
		ComboCount = 0;
		ComboResetTime = 0;
		PlayerHp = PlayerMaxHp;
	}
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::StopFire);
	//InputComponent->BindAction("Dash", IE_Pressed, this, &AFPSCharacter::Dash);
	InputComponent->BindAction("TimeBoom", IE_Pressed, this, &AFPSCharacter::TimeBoom);
	InputComponent->BindAction("Stop", IE_Pressed, this, &AFPSCharacter::TimeStop);

}

void AFPSCharacter::MoveForward(float AxisValue) {
	if (Controller)
	{
		FRotator ControlRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);

		FVector WorldDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(WorldDirection, AxisValue);
	}
}

void AFPSCharacter::MoveRight(float AxisValue) {
	if (Controller)
	{
		FRotator ControlRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);

		FVector WorldDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
	
		AddMovementInput(WorldDirection, AxisValue);
	}
}


void AFPSCharacter::StartJump() {
	bPressedJump = true;
}

void AFPSCharacter::StopJump() {
	bPressedJump = false;
}

void AFPSCharacter::StartFire() {
	IsFire = true;
}

void AFPSCharacter::StopFire() {
	IsFire = false;
}


void AFPSCharacter::Fire() {
	if (ProjectileClass) {
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation = CameraRotation;

		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>
				(ProjectileClass, MuzzlePosition, MuzzleRotation, SpawnParams);

			if (Projectile) {
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				if (GetMesh()) {
					if (GetMesh()->GetAnimInstance()) {
						GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
					}
				}
				PlayShootSound = true;
			}

		}
	}
}

void AFPSCharacter::Dash() {
	FVector VelDir = GetVelocity().GetSafeNormal2D();
	if (VelDir.Size() == 0)
	{
		VelDir = GetControlRotation().Vector().GetSafeNormal2D();
	}
	const FVector LaunchVel = VelDir * 10000.f;
	LaunchCharacter(LaunchVel, true, false);

}

void AFPSCharacter::TimeBoom() {
	if (BoomClass && ComboCount >= 10) {
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		MuzzleRotation = CameraRotation;

		//MuzzleRotation.Pitch += 5.0f;
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			AFPSBoom* Projectile = World->SpawnActor<AFPSBoom>
				(BoomClass, MuzzlePosition, MuzzleRotation, SpawnParams);

			if (Projectile) {
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				SetComboCount(ComboCount - 10);
			}

		}
	}
}

void AFPSCharacter::TimeStop() {
	
	if (ComboCount > 0) {
		IsStop = !IsStop;
		//ComboResetTime = ComboResetDelay;
	}
}

void AFPSCharacter::SetComboCount(int Combo) {
	if (Combo < 0)
		ComboCount = 0;
	else if (Combo > MaxComboCount)
		ComboCount = MaxComboCount;
	else 
		ComboCount = Combo;

	ComboResetTime = ComboResetDelay;

	PlayerAD = 5 + (ComboCount);
}

void AFPSCharacter::AddExp(float xp) {
	if (xp > MaxExp - Exp) {
		Level += 1;
		Exp = 0;
		float Temp = MaxExp;
		MaxExp = Level * 100;
		MaxComboCount = Level * 10;
		AddExp(xp - (Temp - Exp));
		return;
	}	
	else {
		Exp = Exp + xp;

		if (Exp >= MaxExp && Level < MaxLevel) {
			Level += 1;
			Exp = 0;
			MaxExp = Level * 100;
			MaxComboCount = Level * 10;
		}
	}
}