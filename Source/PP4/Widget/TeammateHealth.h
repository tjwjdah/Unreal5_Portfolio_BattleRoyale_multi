// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "TeammateHealth.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UTeammateHealth : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> Pawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTeammateIcon> TeammateIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UProgressBar> HProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> PlayerName;

	FTimerHandle TimerHandle;
protected:
	virtual void NativeConstruct();

public:

	void SetID(int32 _ID) { ID = _ID; }
	void SetPawn(APawn* _Pawn) { Pawn = _Pawn; }
	void SetName(FName _Name) { Name = _Name; }

	int32 GetID() { return ID; }

	void UpdateHealth(float _Health, bool _IsDown);

	void CheckPawnTimer();
};
