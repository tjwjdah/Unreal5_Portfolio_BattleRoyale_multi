// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEntry.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UPlayerEntry : public UUserWidget
{
	GENERATED_BODY()
	private:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText PlayerName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText PlayerPing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> PlayerNameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> PlayerPingText;

protected:
	virtual void NativeConstruct() override;


public:
	void UpdatePing(FText _Text);

	int32 GetID() { return ID; }
	void SetID(int32 _ID) {ID = _ID;}

	void SetPlayerName(FText _Text) { PlayerName = _Text; }

	void SetPlayerPing(FText _Ping) { PlayerPing = _Ping; }
};
