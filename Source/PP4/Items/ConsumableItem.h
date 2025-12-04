// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AConsumableItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	AConsumableItem();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float  HealingValue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EHealingType>  HealingType;

public:
	virtual void BeginPlay() override;

	float GetHealingValue() { return HealingValue;}

	EHealingType GetHealingType() { return HealingType; }
};
