// Fill out your copyright notice in the Description page of Project Settings.


#include "POIW.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
void UPOIW::NativeConstruct()
{
	Super::NativeConstruct();
	if (!bInit)
		Init();
}



void UPOIW::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	TipText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPOIW::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	TipText->SetVisibility(ESlateVisibility::Hidden);
}


void UPOIW::Init()
{
	bInit = true;

	Image->SetBrushFromTexture(Texture);
	Image->SetBrushSize(TextureSize);
	Image->SetBrushTintColor(ColorX);
	TipText->SetText(ToolTip);
	if (ToolTip.IsEmpty())
		VBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

void UPOIW::OutOfMap(bool _Out, bool _MinimapRotate, FVector _LastKnownLocation, FRotator _LastKnowControlRotation)
{
	LastKnownLoc = _LastKnownLocation;
	LastKnownRot = _LastKnowControlRotation;
	if (IsValid(OutOfMapTexture) && IsMinimapPersistent) {
		if (_Out) {
			if (!IsOut)
			{
				IsOut = true;
				Image->SetBrushFromTexture(OutOfMapTexture);
			}
		}
		else {
			if (IsOut)
			{
				IsOut = false;
				Image->SetBrushFromTexture(Texture);
				Image->SetBrushSize(TextureSize);
			}
		}
	}
	SetRotation(_MinimapRotate, _Out);
}

void UPOIW::SetRotation(bool _MinimapRotate, bool _Out)
{
	float Angle = _MinimapRotate ? LastKnownRot.Yaw : 0.f;
	SetRenderTransformAngle(Angle);
	TempAngle = _Out ? GetLookRot() - Angle : (FollowActorRotation ? ActorRef->GetActorRotation().Yaw - Angle : 0.f);
	MainBorder->SetRenderTransformAngle(TempAngle);
	AdjustPadding(_MinimapRotate, _Out);
}

float UPOIW::GetLookRot()
{
	TempLookAt = UKismetMathLibrary::FindLookAtRotation(ActorRef->GetActorLocation(), LastKnownLoc).Yaw;

	return TempLookAt;
}

void UPOIW::AdjustPadding(bool _MinimapRotate, bool _Out)
{
	if (IsMinimapPersistent) {
		FMargin InPadding; InPadding.Bottom = _Out ? TextureSize.Y / 2.f : 0.f;
		UWidgetLayoutLibrary::SlotAsBorderSlot(Image)->SetPadding(InPadding);
		Image->SetRenderTransformAngle((KeepImageStraight && _Out) ? (_MinimapRotate ? (LastKnownRot.Yaw - TempLookAt) : 360.f - TempAngle) : 0.f);
	}
	if (ShowHeightIndicator)
		HandleHeightIndic(_MinimapRotate, _Out);
}

void UPOIW::HandleHeightIndic(bool _MinimapRotate, bool _Out)
{
	float HeightDiff = LastKnownLoc.Z - ActorRef->GetActorLocation().Z;
	if (_Out || abs(HeightDiff) < AcceptableHeightDiff) {
		DownArrow->SetVisibility(ESlateVisibility::Hidden);
		UpArrow->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		if (HeightDiff > 0.f) {
			DownArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UpArrow->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			DownArrow->SetVisibility(ESlateVisibility::Hidden);
			UpArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UPOIW::ChangeColor(FSlateColor _Color)
{
	ColorX = _Color;
	Image->SetBrushTintColor(_Color);
}

void UPOIW::ChangeTexture(UObject* _Image, FVector2D _ImageSize)
{
	FSlateBrush InBrush = Image->GetBrush();
	InBrush.ImageSize = _ImageSize;
	InBrush.SetResourceObject(_Image);
	Image->SetBrush(InBrush);
}

void UPOIW::ChangeTextureSize(FVector2D _Size)
{
	Image->SetBrushSize(_Size);
}

void UPOIW::ChangeHeightIndicSettings(UTexture2D* _Texture1, UTexture2D* _Texture2, FSlateColor _Color1, FSlateColor _Color2, FVector2D _Size, float _AccepDiff)
{
	AcceptableHeightDiff = _AccepDiff;
	UpArrow->SetBrushFromTexture(_Texture1);
	UpArrow->SetBrushSize(_Size);
	UpArrow->SetBrushTintColor(_Color1);

	DownArrow->SetBrushFromTexture(_Texture2);
	DownArrow->SetBrushSize(_Size);
	DownArrow->SetBrushTintColor(_Color2);
}
