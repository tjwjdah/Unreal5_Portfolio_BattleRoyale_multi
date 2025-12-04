// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "OnDeathMatchDeath.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UOnDeathMatchDeath : public UUserWidget
{
	GENERATED_BODY()
	private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CurrentCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> Counter;

	FTimerHandle TimerHandle;
protected:
	virtual void NativeConstruct() override;


	void OnCountDonw();
};
