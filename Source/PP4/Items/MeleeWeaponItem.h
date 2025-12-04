// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "MeleeWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AMeleeWeaponItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	AMeleeWeaponItem();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundCue> MeleeHitSound;

public:
	virtual void BeginPlay() override;

	float GetDamage() { return Damage; }

	USoundCue* GetMeleeHitSound() { return MeleeHitSound; }
};
