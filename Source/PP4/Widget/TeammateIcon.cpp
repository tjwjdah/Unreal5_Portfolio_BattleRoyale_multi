// Fill out your copyright notice in the Description page of Project Settings.


#include "TeammateIcon.h"
#include "InGameWidget.h"

#include "Components/CanvasPanelSlot.h"


void UTeammateIcon::NativeConstruct()
{
	Super::NativeConstruct();


}


void UTeammateIcon::Initialize(FLinearColor _Color)
{
	PositionIcon->SetColorAndOpacity(_Color);
}
