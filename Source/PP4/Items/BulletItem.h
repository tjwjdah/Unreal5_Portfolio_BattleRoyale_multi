// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "BulletItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABulletItem : public APickupItem
{
	GENERATED_BODY()
	
	
public:
	virtual void BeginPlay() override;
};
