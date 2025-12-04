// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "KillFeed.h"
#include "TeammateHealth.h"

#include "../Character/BRCharacter.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"

void UInGameWidget::NativeConstruct()
{
    Super::NativeConstruct();
   if(UGameplayStatics::GetPlayerController(GetWorld(),0)->HasAuthority())
       TextInfo2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}


bool UInGameWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    UTexture2D* Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Assetss/3-WidgetBP/Images/Crosshair2.Crosshair2'"));
    if (Texture)
        CrosshairTextures.Add(Texture);

    Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Assetss/3-WidgetBP/Images/Dot.Dot'"));
    if (Texture)
        CrosshairTextures.Add(Texture);

    return true;
}

void UInGameWidget::GameStarted()
{
    TextInfo1->SetVisibility(ESlateVisibility::Hidden);
    TextInfo2->SetVisibility(ESlateVisibility::Hidden);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    TimerHandle.Invalidate();
    TimerOn = false;
}

void UInGameWidget::GameStarting(int32 _Time, bool _Starting)
{
    //GameStatus != EGameStatus::Waiting
   if (_Starting) {
        TimeStart = _Time > 0 ? _Time - 1 :0;
        TextInfo1->SetText(FText::FromString(FString(TEXT("Game starting in : "))));
        TextInfo2->SetText(FText::AsNumber(TimeStart));
        TextInfo1->SetVisibility(ESlateVisibility::HitTestInvisible);
        TextInfo2->SetVisibility(ESlateVisibility::HitTestInvisible);
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle, [this]() {
                TimeRemainingTimer();}, 1.f, true);
        TimerOn = true;
    }
    else {
       if (TimerOn) {
           GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
           TimerHandle.Invalidate();
           TimerOn = false;
       }
       TextInfo1->SetText(FText::FromString(FString(TEXT("Waiting for players.."))));
       TextInfo2->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInGameWidget::UpdatePlayersAlive(int32 _Alive)
{
    AliveCounter->SetText(FText::AsNumber(_Alive));
}

void UInGameWidget::WaitingForPlayers()
{
    TextInfo2->SetVisibility(ESlateVisibility::Hidden);
    TextInfo1->SetText(FText::FromString(FString(TEXT("Waiting for players.."))));
    TextInfo1->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInGameWidget::AddKillFeed(const FText& _Killer, const FText& _Killed, const FText& _WeaponUsed)
{
    UKillFeed* Kill = Cast<UKillFeed>(CreateWidget(GetWorld(), KillFeedClass));
    Kill->SetKilled(_Killed);
    Kill->SetKiller(_Killer);
    Kill->SetWeaponUsed(_WeaponUsed);
    KillFeed->AddChild(Kill);
    if (KillFeed->GetChildrenCount() > 5)
        KillFeed->RemoveChildAt(0);
}

void UInGameWidget::ShowBottomInfo(const FText& _Info1, const FText& _Info2, FSlateColor _Color1, FSlateColor _Color2, float _Duration)
{
    BottomInfo1->SetText(_Info1);
    BottomInfo2->SetText(_Info2);
    InfoBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
    BottomInfo2->SetVisibility(ESlateVisibility::HitTestInvisible);
    BottomInfo1->SetColorAndOpacity(_Color1);
    BottomInfo2->SetColorAndOpacity(_Color2);
    Hide = false;

    if (_Duration > 0.f) {
        Hide = true;
        GetWorld()->GetTimerManager().SetTimer(
            RetriggerableTimerHandle, [this]() {
                if (Hide) {
                    Hide = false;
                    HideBottomInfo();
                }
            }, _Duration, false);
    }

}

void UInGameWidget::ZoneStarting(int32 _Seconds)
{
    ZoneTime = _Seconds;
    FText Text = FText::Format(FText::FromString("Zone moving in {0} seconds.."), FText::AsNumber(_Seconds));
    ZoneText->SetText(Text);
    SetZoneCountTimer();
    ZoneText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInGameWidget::ZonePaused(int32 _Seconds)
{
    ZoneStarting(_Seconds);
}

void UInGameWidget::ZoneEnded()
{
    ZoneText->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameWidget::ZoneMoving()
{
    GetWorld()->GetTimerManager().ClearTimer(ZoneTimeHandle);
    ZoneTimeHandle.Invalidate();
    FText Text = FText::FromString("Zone moving..");
    ZoneText->SetText(Text);
    ZoneText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInGameWidget::ZoneSuspended()
{
    GetWorld()->GetTimerManager().ClearTimer(ZoneTimeHandle);
    ZoneTimeHandle.Invalidate();
    ZoneText->SetText(FText::FromString(TEXT("Zone stopped by Admin")));
}

void UInGameWidget::ClearActiveSlot()
{
    WeaponBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameWidget::UpdateBoost(float _Boost)
{
    BoostProgressbar->SetPercent(_Boost / 100.f);
}

void UInGameWidget::UpdateWater(float _Water)
{
    WaterProgressBar->SetPercent(_Water / 100.f);
}

void UInGameWidget::ShowHitMarker()
{
    HitMarkerImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    GetWorld()->GetTimerManager().SetTimer(
        ShowHitMarkerHandle, [this]() {
            HitMarkerImg->SetVisibility(ESlateVisibility::Hidden);
        }, 0.2, false);

}

void UInGameWidget::HideBottomInfo()
{
    if (!Hide) {
        InfoBorder->SetVisibility(ESlateVisibility::Hidden);
        Hide = true;
    }
}

void UInGameWidget::StartRevive(float _ReviveDurationSec, bool _Reviving)
{
    ReviveTeamVB->SetVisibility(ESlateVisibility::Visible);
    RevivePercent->SetPercent(0.f);
    FText Text = _Reviving ? FText::FromString(TEXT("Reviving teammate..")) : FText::FromString(TEXT("You are being revived..."));
    BottomInfo3->SetText(Text);
    PlayAnimation(ReviveProgress, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f / _ReviveDurationSec);
    Reviving = _Reviving;
    FWidgetAnimationDynamicEvent MyAnimationEvent;
    MyAnimationEvent.Clear();
    MyAnimationEvent.BindDynamic(this, &UInGameWidget::ReviveProgressAnimFinished);
    BindToAnimationFinished(ReviveProgress, MyAnimationEvent);
}

void UInGameWidget::UpdateTeammateHealth(int32 _ID, float _Health, bool _IsDown, FVector _PlayerLoc)
{
    if (TeammatesHealthBox->GetChildrenCount() > 0) {
        for (int32 i = 0; i < TeammatesHealthBox->GetChildrenCount(); i++) {
            CV(UTeammateHealth, Team, TeammatesHealthBox->GetChildAt(i));
            if (!IsValid(Team))continue;
            if (Team->GetID() == _ID)
            {
                Team->UpdateHealth(_Health, _IsDown);
                break;
            }
        }
    }
}

void UInGameWidget::AddTeammateHealthBar(int32 _ID, APawn* _Pawn, FName _Name, FSlateColor _Color)
{
    UTeammateHealth* TeammateHealth = Cast<UTeammateHealth>(CreateWidget(GetWorld(), TeammateHealthClass));
    TeammateHealth->SetID(_ID);
    TeammateHealth->SetPawn(_Pawn);
    TeammateHealth->SetName(_Name);
    TeammateHealth->SetOwningPlayer(GetOwningPlayer());
    TeammatesHealthBox->AddChild(TeammateHealth);
    TeammateHealth->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
}                  

void UInGameWidget::UpdateAmmoCount(int32 _Current, int32 _Total)
{
    FText Text = FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(_Current), FText::AsNumber(_Total));
    AmmoText->SetText(Text);
}

void UInGameWidget::UpdateNadeCounts(int32 _Grenade, int32 _Smoke, int32 _Stun)
{
    GreandeCount->SetText(FText::AsNumber(_Grenade));
    SmokeCount->SetText(FText::AsNumber(_Smoke));
    StunCount->SetText(FText::AsNumber(_Stun));
}

void UInGameWidget::UpdateKills(int32 _Kills)
{
    KillsCounter->SetText(FText::AsNumber(_Kills));
    KillsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UInGameWidget::SetActiveExplosive(EPickupType _Type)
{
    FLinearColor Color = SmokeBorder->GetBrushColor();
    Color.A = 0.f;
    SmokeBorder->SetBrushColor(Color);
    FragBorder->SetBrushColor(Color);
    StunBorder->SetBrushColor(Color);
    Color.A = 1.f;
    if(_Type == EPickupType::FragGrenade)
        FragBorder->SetBrushColor(Color);
    else if (_Type == EPickupType::SmokeGrenade)
        SmokeBorder->SetBrushColor(Color);
    else if (_Type == EPickupType::StunGrenade)
        StunBorder->SetBrushColor(Color);
}

void UInGameWidget::StopRevive()
{
    UnbindAllFromAnimationFinished(ReviveProgress);
    //Unreal Engine에서 애니메이션 몽타주나 애니메이션 시퀀스 재생이 끝났을 때 호출되는 **delegate(델리게이트)**에 등록된 모든 함수를 **해제(unbind)**하는 함수입니다.
    StopAnimation(ReviveProgress);
    ReviveTeamVB->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameWidget::SetPlayerID(FString _Name)
{
    PlayerIDText->SetText(FText::FromString(_Name));
}

void UInGameWidget::ChangeCrosshair(ECrossHair _CrossHair, FVector2D _ImageSize)
{
   
    Crosshair->SetBrushSize(_ImageSize);
    Crosshair->SetBrushResourceObject(CrosshairTextures[static_cast<int32>(_CrossHair)]);
}

void UInGameWidget::UpdateActiveSlot(UTexture2D* _Image, int32 _CurrentAmmo, int32 _TotalAmmo)
{
    EquipedWeapon->SetBrushFromTexture(_Image, false);
    FText Text = _TotalAmmo == -2 ? FText::FromString(TEXT("-- / --")) : FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(_CurrentAmmo), FText::AsNumber(_TotalAmmo));
    AmmoText->SetText(Text);
    WeaponBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UInGameWidget::TimeRemainingTimer()
{
    TimeStart--;
    TextInfo2->SetText(FText::AsNumber(TimeStart));
    if (TimeStart <= 0) {
        TextInfo1->SetVisibility(ESlateVisibility::Hidden);
        TextInfo2->SetVisibility(ESlateVisibility::Hidden);
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        TimerHandle.Invalidate();
        TimerOn = false;
    }
}

void UInGameWidget::SetZoneCountTimer()
{
    if (ZoneTimeHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(ZoneTimeHandle);
        ZoneTimeHandle.Invalidate();
    }
    GetWorld()->GetTimerManager().SetTimer(
        ZoneTimeHandle, [this]() {
            ZoneTime--;
            if (ZoneTime <= 0) {
                GetWorld()->GetTimerManager().ClearTimer(ZoneTimeHandle);
                ZoneTimeHandle.Invalidate();
            }
            else
            {
                FText Text = FText::Format(FText::FromString("Zone moving in {0} seconds.."), FText::AsNumber(ZoneTime));
                ZoneText->SetText(Text);
            }
        }, 1.f, true);

}

void UInGameWidget::ReviveProgressAnimFinished()
{
    if (Reviving) {
        CV(ABRCharacter, Char, GetOwningPlayer()->GetPawn());
        if (!IsValid(Char))return;
        Char->OnReviveProgressDone();
    }
    ReviveTeamVB->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameWidget::UpdateHealth(float _NewHealth, bool _PlayerDown)
{
    HealthProgress->SetPercent(_NewHealth / 100.f);
    if (_NewHealth < 20.f || _PlayerDown) 
        HealthProgress->SetFillColorAndOpacity(FLinearColor(1.f, 0.f, 0.f));
    else if(_NewHealth < 60.f)
        HealthProgress->SetFillColorAndOpacity(FLinearColor(0.48f, 1.f, 0.13f));
    else
        HealthProgress->SetFillColorAndOpacity(FLinearColor(0.f, 1.f, 0.22f));
}
