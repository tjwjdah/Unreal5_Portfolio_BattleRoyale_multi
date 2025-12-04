// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "ArmorItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AArmorItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	AArmorItem();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ArmorValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> EquipSeletalMesh;

public:
	virtual void BeginPlay() override;

	USkeletalMesh* GetEquipSeletalMesh() { return EquipSeletalMesh; }

	float TakeDamage(float _Damage);

	float GetArmorValue() { return ArmorValue; }
	void SetArmorValue(float _Value) { ArmorValue = _Value; }
};
