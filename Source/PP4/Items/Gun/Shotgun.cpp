// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "../../Character/BRCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AShotgun::AShotgun()
{
	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagMesh"));
	MagMesh->SetupAttachment(GetStaticMesh());

	PerShellDamage = 10.f;

	NumberShells = 7;

	ShootGunRange = 15000.f;

	DoOnce = true;
}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

bool AShotgun::Fire()
{
	/*if (!CanFire())
	return false;
	FTransform Trans;
	auto* MeshRef = GetMeshComponentRef(false, Trans);
	FVector CrosshairCenter = GetProjectileDestination();
	FVector MuzzleLocation = MeshRef->GetSocketLocation(TEXT("Muzzle"));

	for (int32 i = 0; i <= NumberShells; i++) {
		int32 Index = i;
		FHitResult OutHit;
		FVector Start = MuzzleLocation;
		FRotator Rot = (CrosshairCenter - MuzzleLocation).Rotation();
		Rot.Pitch+= UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f);
		Rot.Yaw += UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f);
		FVector End = MuzzleLocation+(Rot.Vector()*ShootGunRange);

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = false;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(GetWeaponOwner());
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			OutHit,
			Start,
			End,
			ECC_Visibility,
			TraceParams
		);
		if (bHit) {
			CV(ACharacter, Char, OutHit.GetActor());
			if (IsValid(Char) && DoOnce) {
				DoOnce = false;
				MCSpawnBloodEffectAtLocation(OutHit.Location);
			}
			CV(ABRCharacter, Char2, OutHit.GetActor());
			if (!IsValid(Char2)) {
				if(IsValid(OutHit.GetActor()))
					UGameplayStatics::ApplyDamage(OutHit.GetActor(),PerShellDamage,DamageCau)
			}
		}
	}
	*/
	return false;
}

float AShotgun::CalculateDamage(const FName _BoneName, ABRCharacter* _Char)
{
	return 0.0f;
}

void AShotgun::CalculateArmorDamage(float _Damage, float _Armor, float& _DamageTaken, float& _ArmorRemaining)
{
}

void AShotgun::MCSpawnHitEffectAtLocations_Implementation(const TArray<FVector>& _Vectors)
{
	for (int32 i = 0; i < _Vectors.Num(); i++) {
		FTransform Trans; Trans.SetLocation(_Vectors[i]); Trans.SetScale3D(FVector(1.f, 1.f, 1.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Trans);
	}
}

void AShotgun::MCSpawnBloodEffectAtLocation_Implementation(FVector _Vector)
{
	FTransform Trans; Trans.SetLocation(_Vector); Trans.SetScale3D(FVector(1.f, 1.f, 1.f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, Trans);
}
