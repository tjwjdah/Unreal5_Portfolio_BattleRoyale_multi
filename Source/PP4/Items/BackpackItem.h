// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "BackpackItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABackpackItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	ABackpackItem();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Capacity;

public:
	virtual void BeginPlay() override;

	int32 GetLevel() { return Level; }

	float GetCapacity() { return Capacity; }
};
