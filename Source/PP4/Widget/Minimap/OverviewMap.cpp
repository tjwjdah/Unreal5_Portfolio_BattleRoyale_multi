// Fill out your copyright notice in the Description page of Project Settings.


#include "OverviewMap.h"
#include "MinimapComponent.h"
void UOverviewMap::NativeConstruct()
{
	Super::NativeConstruct();

	MinimapComponent->SetIsBigMap(true);
}
