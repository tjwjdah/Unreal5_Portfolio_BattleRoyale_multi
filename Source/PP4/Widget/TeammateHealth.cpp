// Fill out your copyright notice in the Description page of Project Settings.


#include "TeammateHealth.h"

#include "../Main/BRGameState.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
void UTeammateHealth::NativeConstruct()
{
    Super::NativeConstruct();
	PlayerName->SetText(FText::FromName(Name));

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, [this]() {
            CheckPawnTimer();
        }, 1.f, true);
 
}



void UTeammateHealth::UpdateHealth(float _Health, bool _IsDown)
{
    HProgressBar->SetPercent(_Health / 100.f);
    HProgressBar->SetFillColorAndOpacity(_Health < 26.f || _IsDown ? FLinearColor(1.f, 0.f, 0.f) : FLinearColor(0.f, 1.f, 0.29f));
}

void UTeammateHealth::CheckPawnTimer()
{
    if (IsValid(Pawn))return;

    CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
    if (!State)return;
    
    if (State->GetGameStatus() == EGameStatus::Waiting)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        TimerHandle.Invalidate();
        RemoveFromParent();
    }
    else {
        HProgressBar->SetPercent(0.f);
        FSlateBrush Brush;
        Brush.ImageSize = FVector2D(32.f, 32.f);
        Brush.TintColor = FSlateColor(FLinearColor::Red);
        Brush.DrawAs = ESlateBrushDrawType::Image;
        Brush.Tiling = ESlateBrushTileType::NoTile;
        Brush.Mirroring = ESlateBrushMirrorType::NoMirror;
        Brush.Margin = FMargin(0.0f);
        HProgressBar->WidgetStyle.FillImage = Brush;
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        TimerHandle.Invalidate();
    }
}
