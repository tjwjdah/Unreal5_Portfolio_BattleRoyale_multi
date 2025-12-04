// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEntry.h"

#include "Components/TextBlock.h"

void UPlayerEntry::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerNameText->SetText(PlayerName);
	PlayerPingText->SetText(PlayerPing);
}

void UPlayerEntry::UpdatePing(FText _Text)
{
	PlayerPingText->SetText(_Text);
}
