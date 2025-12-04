// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"

AConsumableItem::AConsumableItem()
{
	HealingType = EHealingType::Health;
	HealingValue = 40.f;
}

void AConsumableItem::BeginPlay()
{
	Super::BeginPlay();
}
