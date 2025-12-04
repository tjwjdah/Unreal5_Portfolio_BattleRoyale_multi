// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ConsumeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UConsumeWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> LoadingAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AnimationTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 UniqueServerID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRCharacter> PlayerOwner;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> ItemName;


protected:
	virtual void NativeConstruct();
	
public:
	FText GetItemName() { return Name; }
	void SetAnimationTime(float _AnimationTime) { AnimationTime = _AnimationTime; }
	void SetUniqueServerID(int32 _UniqueServerID) { UniqueServerID = _UniqueServerID; }
	void SetPlayerOwner(ABRCharacter* _PlayerOwner) { PlayerOwner = _PlayerOwner; }
	void SetItemName(FText _Text) { Name = _Text; }

	UFUNCTION()
	void AnimationDone();
};
