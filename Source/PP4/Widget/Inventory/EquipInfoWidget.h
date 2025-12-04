// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"

#include "Blueprint/UserWidget.h"
#include "EquipInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UEquipInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UItemsContainer> BackpackContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UItemsContainer> BodyArmorContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UItemsContainer> HeadArmorContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UItemsContainer> MeleeContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UItemsContainer> PistolContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UItemsContainer> PrimaryContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UItemsContainer> SecondaryContainer;


	protected:
	virtual void NativeConstruct();
	
public:

	class UItemsContainer* GetContainer(ESlotType _Num);

	void PickupEquip(ESlotType _Slot, const FPickupInfos& _Info);

};
