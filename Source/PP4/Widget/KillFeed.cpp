// Fill out your copyright notice in the Description page of Project Settings.


#include "KillFeed.h"

#include "Components/TextBlock.h"
void UKillFeed::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerKillerText->SetText(Killer);
	PlayerKilledText->SetText(Killed);
	WeaponText->SetText(WeaponUsed);

	if (Killer.ToString() == TEXT("DC")) {
		KilledText->SetVisibility(ESlateVisibility::Hidden);
		PlayerKillerText->SetVisibility(ESlateVisibility::Hidden);
		WithText->SetVisibility(ESlateVisibility::Hidden);
		KilledText->SetText(FText::FromString(TEXT("")));
		PlayerKillerText->SetText(FText::FromString(TEXT("")));
		WithText->SetText(FText::FromString(TEXT("")));
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
		RemoveFromParent();
		}, 4.f, false);
}

