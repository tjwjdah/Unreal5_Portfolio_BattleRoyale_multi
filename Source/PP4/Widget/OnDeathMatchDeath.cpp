// Fill out your copyright notice in the Description page of Project Settings.


#include "OnDeathMatchDeath.h"

#include "Components/TextBlock.h"
void UOnDeathMatchDeath::NativeConstruct()
{
    Super::NativeConstruct();
	CurrentCount = 10;

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,this,&UOnDeathMatchDeath::OnCountDonw, 1.f, true);
}

void UOnDeathMatchDeath::OnCountDonw()
{
    CurrentCount--;
    Counter->SetText(FText::AsNumber(CurrentCount));
    if (CurrentCount <= 0) {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        TimerHandle.Invalidate();
    }
}
