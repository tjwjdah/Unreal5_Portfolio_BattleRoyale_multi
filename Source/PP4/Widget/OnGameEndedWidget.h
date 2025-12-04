// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "OnGameEndedWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UOnGameEndedWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Rank;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> SpectatingPlayer;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UButton> ExitBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UHorizontalBox> HorizontalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> LobyBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> MessageText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> RankText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> SpectateTeammateBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> SpectateRandomBtn;

protected:
	virtual void NativeConstruct() override;


public:
	void SetUpSpectating(APawn* _SpectatingPlayer);

	void SetRank(int32 _Rank) { Rank = _Rank; }

	UFUNCTION()
	void OnSpectateTeammateBtnClicked();

	UFUNCTION()
	void OnSpectateRandomBtnClicked();

	UFUNCTION()
	void OnLobyBtnClicked();

	UFUNCTION()
	void OnExitBtnClicked();
};
