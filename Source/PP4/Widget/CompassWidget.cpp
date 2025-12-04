// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
void UCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CompassSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CompassImage);
	
}

void UCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!IsValid(Controller))return;
	FRotator Rot(180.f, 180.f, 180.f);
	CompassSlot->SetPosition(FVector2D((Rot - Controller->GetControlRotation()).Yaw * -10.f, 0.f));
}
