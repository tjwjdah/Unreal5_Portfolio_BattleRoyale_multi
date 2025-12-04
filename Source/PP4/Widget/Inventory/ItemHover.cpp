// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHover.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Framework/Application/SlateApplication.h"
void UItemHover::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UItemHover::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Parent->IsVisible() && !IsValid(Parent)) {
		RemoveFromParent();
		return;
	}
	
}


void UItemHover::UpdateWeightText(FText _Text)
{
	WeightAmount->SetText(_Text);
}



void UItemHover::UpdateInfo(FText _Name, FText _Description, UObject* _Image)
{
	ItemNameText->SetText(_Name);
	ItemDesc->SetText(Description);

	ItemImage->SetBrushResourceObject(_Image);
}

void UItemHover::InitPos()
{
	float XMousePosition; float YMousePosition; 
	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(XMousePosition, YMousePosition)) 
	{ return; }


	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x : %f y : %f"), XMousePosition, YMousePosition));

	int32 SizeX; int32 SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);
	FVector2D ScreenPos;
	ScreenPos.X = XMousePosition + 30.f + ((SizeX - XMousePosition) < (SizeX / 3.f) ? MainBorder->GetCachedGeometry().GetLocalSize().X * -1.2f : 0.f);
	ScreenPos.Y = (YMousePosition - SizeY / 4.f) - 30.f + ((SizeY - YMousePosition) > (SizeY / 3.f) ? MainBorder->GetCachedGeometry().GetLocalSize().Y * 1.2f : 0.f);

	SetPositionInViewport(ScreenPos);
	
}

void UItemHover::WidgetFadeOut()
{
	/*
	if (GetColorAndOpacity().A != 1.f) {
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}*/
	FadingOut = true;
	PlayAnimation(FadeOut);
	FTimerHandle TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] {
		if (FadingOut)
			RemoveFromParent();
		}, 0.2f, false);

}

void UItemHover::WidgetFadeIn()
{
	//InitPos();
	FadingOut = false;
	StopAnimation(FadeOut);
	PlayAnimation(FadeIn);
}

void UItemHover::Init()
{
	ItemNameText->SetText(ItemName);
	ItemDesc->SetText(Description);
	ItemImage->SetBrushResourceObject(Image);
	WeightAmount->SetText(Weight);

	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PC->bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());         // 포커스는 여전히 UI
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);                // 드래그 중에도 커서 숨기지 않기

	PC->SetInputMode(InputMode);
}
