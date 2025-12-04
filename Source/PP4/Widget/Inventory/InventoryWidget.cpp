// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "ItemsContainer.h"
#include "ItemWidget.h"

#include "../../Etc/InventorySystem/CharacterPreview.h"
#include "../../Character/BRCharacter.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Framework/Application/SlateApplication.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CV(ABRCharacter, Char, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (IsValid(Char)) {
		CharacterPrev = Char->GetPrevCharacter();
		CharacterPrev->TogglePreview(true);
	}
}


void UInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(CharacterPrev))CharacterPrev->TogglePreview(false);
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

}

FReply UInventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	PrevBtnDown = false;
	return FReply::Handled();
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (PrevBorder && PrevBorder->IsHovered())
	{
		float x, y;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGameplayStatics::GetPlayerController(GetWorld(), 0), x, y);
		LastSavedX = x;
		PrevBtnDown = true;
		return FReply::Handled();
	}

	PrevBtnDown = false;

	return FReply::Handled();
	
}

FReply UInventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	if(!PrevBtnDown || !IsValid(CharacterPrev))
		return FReply::Unhandled();
	float x, y;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGameplayStatics::GetPlayerController(GetWorld(), 0), x, y);
	float NewLastSavedX = x;
	x -= LastSavedX;
	x *= -1.f;
	CharacterPrev->RotationCharacter(x);
	LastSavedX = NewLastSavedX;

	return FReply::Unhandled();
}



UItemsContainer* UInventoryWidget::GetContainer(ESlotType _Type)
{
	switch (_Type)
	{
	case ESlotType::Inventory:
		return MainItemsContainer;
		break;
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
	case ESlotType::Consumable:
		return ConsumableContainer;
		break;

	}
    return nullptr;
}

void UInventoryWidget::UpdateInventoryWeight(float _Current, float _Max)
{
	FText Text = FText::Format(FText::FromString("{0} / {m}"), _Current,_Max);
	WeightAmount->SetText(Text);
}

void UInventoryWidget::PickupEquip(ESlotType _Slot,const FPickupInfos& _Info)
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
	case ESlotType::Consumable:
		ConsumableContainer->UpdateSlot(_Info, 0, ESlotType::Consumable, 0);
		break;
	}
}

void UInventoryWidget::UpdateArmorValue(ESlotType _Slot, float _Value)
{
	if (ESlotType::HeadArmor == _Slot) 
		HeadArmorContainer->GetSlotIndexItemWidget(0)->UpdateArmorValue(_Value);
	else if(ESlotType::BodyArmor == _Slot)
		BodyArmorContainer->GetSlotIndexItemWidget(0)->UpdateArmorValue(_Value);
}

void UInventoryWidget::UpdateBackPackLevel(int32 _Lv)
{
	FText Text = FText::Format(FText::FromString("[Lv:{0}]"),FText::AsNumber(_Lv));
	BackLv->SetText(Text);
}

