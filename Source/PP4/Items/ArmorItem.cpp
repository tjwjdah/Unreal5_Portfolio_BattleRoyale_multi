// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItem.h"

AArmorItem::AArmorItem()
{
	ArmorValue = 100.f;
}

void AArmorItem::BeginPlay()
{
	Super::BeginPlay();
}

float AArmorItem::TakeDamage(float _Damage)
{
	if (ArmorValue - _Damage < 0)
		ArmorValue = 0.f;
	else
		ArmorValue -= _Damage;
	return ArmorValue;
	
}
