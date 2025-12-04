// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipInfoWidget.h"
#include "ItemsContainer.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UEquipInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}



UItemsContainer* UEquipInfoWidget::GetContainer(ESlotType _Num)
{
	switch (_Num)
	{
	case ESlotType::Primary:
		return PrimaryContainer;
		break;
	case ESlotType::Secondary:
		return SecondaryContainer;
		break;
	case ESlotType::Pistol:
		return PistolContainer;
		break;
	case ESlotType::Melee:
		return MeleeContainer;
		break;
	case ESlotType::HeadArmor:
		return HeadArmorContainer;
		break;
	case ESlotType::BodyArmor:
		return BodyArmorContainer;
		break;
	case ESlotType::Backpack:
		return BackpackContainer;
		break;
	}
    return nullptr;
}

void UEquipInfoWidget::PickupEquip(ESlotType _Slot, const FPickupInfos& _Info)
{
	switch (_Slot)
	{
	case ESlotType::Primary:
		PrimaryContainer->UpdateSlot(_Info, 0, ESlotType::Primary, 0);
		break;
	case ESlotType::Secondary:
		SecondaryContainer->UpdateSlot(_Info, 0, ESlotType::Secondary, 0);
		break;
	case ESlotType::Pistol:
		PistolContainer->UpdateSlot(_Info, 0, ESlotType::Pistol, 0);
		break;
	case ESlotType::Melee:
		MeleeContainer->UpdateSlot(_Info, 0, ESlotType::Melee, 0);
		break;
	case ESlotType::HeadArmor:
		HeadArmorContainer->UpdateSlot(_Info, 0, ESlotType::HeadArmor, 0);
		break;
	case ESlotType::BodyArmor:
		BodyArmorContainer->UpdateSlot(_Info, 0, ESlotType::BodyArmor, 0);
		break;
	case ESlotType::Backpack:
		BackpackContainer->UpdateSlot(_Info, 0, ESlotType::Backpack, 0);
		break;
	}
}
