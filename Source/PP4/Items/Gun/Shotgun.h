// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../WeaponItem.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AShotgun : public AWeaponItem
{
	GENERATED_BODY()
	
public:
	AShotgun();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MagMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PerShellDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumberShells;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ShootGunRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> BloodEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> HitEffect;

	bool DoOnce;
public:
	virtual void BeginPlay() override;
	

	virtual bool Fire() override;

	float CalculateDamage(const FName _BoneName, class ABRCharacter* _Char);

	void CalculateArmorDamage(float _Damage, float _Armor, float& _DamageTaken, float& _ArmorRemaining);

	UFUNCTION(NetMulticast, Unreliable)
	void MCSpawnHitEffectAtLocations(const TArray<FVector>& _Vectors);

	UFUNCTION(NetMulticast, Unreliable)
	void MCSpawnBloodEffectAtLocation(FVector _Vector);

};
