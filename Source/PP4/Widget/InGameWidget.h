// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECrossHair : uint8
{
	CrossHair,
	Dot,
};
UCLASS()
class PP4_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ReviveProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTexture2D>> CrosshairTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> AliveCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UProgressBar> BoostProgressbar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> BottomInfo1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> BottomInfo2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> BottomInfo3;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UImage> Crosshair;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> EquipedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UBorder> StunBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UBorder> FragBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> GreandeCount; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> HealthProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> HitMarkerImg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UBorder> InfoBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UVerticalBox> KillFeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UKillFeed> KillFeedClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UHorizontalBox> KillsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> KillsCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> PlayerIDText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> RevivePercent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UHorizontalBox> ReviveTeamVB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UBorder> SmokeBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> SmokeCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> StunCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UVerticalBox> TeammatesHealthBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeammateHealth> TeammateHealthClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> TextInfo1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> TextInfo2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> TextInfo3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> WaterProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UBorder> WeaponBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> ZoneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 TimeStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool TimerOn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Hide;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ZoneTime;
	
	FTimerHandle ZoneTimeHandle;
	FTimerHandle TimerHandle;
	FTimerHandle RetriggerableTimerHandle;
	FTimerHandle ShowHitMarkerHandle;

	bool Reviving;
protected:
	virtual void NativeConstruct();
	virtual bool Initialize();
	
public:
	void GameStarted();

	void GameStarting(int32 _Time, bool _Starting);

	void UpdatePlayersAlive(int32 _Alive);

	void WaitingForPlayers();

	void AddKillFeed(const FText& _Killer, const FText& _Killed, const FText& _WeaponUsed);

	void ShowBottomInfo(const FText& _Info1, const FText& _Info2, FSlateColor _Color1, FSlateColor _Color2, float _Duration);

	void ZoneStarting(int32 _Seconds);
	void ZonePaused(int32 _Seconds);
	void ZoneEnded();
	void ZoneMoving();
	void ZoneSuspended();

	void ClearActiveSlot();

	void UpdateHealth(float _NewHealth, bool _PlayerDown);
	void UpdateBoost(float _Boost);
	void UpdateWater(float _Water);

	void ShowHitMarker();

	void HideBottomInfo();

	void StartRevive(float _ReviveDurationSec, bool _Reviving);

	void UpdateTeammateHealth(int32 _ID, float _Health, bool _IsDown, FVector _PlayerLoc);
	void AddTeammateHealthBar(int32 _ID, APawn* _Pawn, FName _Name, FSlateColor _Color);

	void UpdateAmmoCount(int32 _Current, int32 _Total);

	void UpdateNadeCounts(int32 _Grenade, int32 _Smoke, int32 _Stun);

	void UpdateKills(int32 _Kills);

	void SetActiveExplosive(EPickupType _Type);

	void StopRevive();

	void SetPlayerID(FString _Name);

	void ChangeCrosshair(ECrossHair _CrossHair, FVector2D _ImageSize);

	void UpdateActiveSlot(UTexture2D* _Image, int32 _CurrentAmmo, int32 _TotalAmmo);

	void TimeRemainingTimer();

	void SetZoneCountTimer();

	UFUNCTION()
	void ReviveProgressAnimFinished();
};


