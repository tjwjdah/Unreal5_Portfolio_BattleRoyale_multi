// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBackground.h"
#include "InventoryDragDropOperation.h"
#include "ItemWidget.h"
#include "ItemsContainer.h"

#include "../../Character/BRCharacter.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/Border.h"

void UInventoryBackground::NativeConstruct()
{
	Super::NativeConstruct();
	if (BtnCancel)
		BtnCancel->OnClicked.AddDynamic(this, &UInventoryBackground::OnCancelClicked);
	
	if (BtnConfirm)
		BtnConfirm->OnClicked.AddDynamic(this, &UInventoryBackground::OnConfirmClicked);
}

void UInventoryBackground::OnCancelClicked()
{
	DropAmountBorder->SetVisibility(ESlateVisibility::Hidden);
	CurrentItemWidget = nullptr;
}

void UInventoryBackground::OnConfirmClicked()
{
	
	if (!AmountInput->GetText().IsEmpty() && AmountInput->GetText().IsNumeric() && FCString::Atoi(*AmountInput->GetText().ToString()) <= CurrentItemWidget->GetPickupInfo().Count) {
		DragDropFromContainer(CurrentItemWidget->GetContainer()->GetContainerType(), CurrentItemWidget->GetUniqueServerID(), FCString::Atoi(*AmountInput->GetText().ToString()));
		DropAmountBorder->SetVisibility(ESlateVisibility::Hidden);
		CurrentItemWidget = nullptr;
	}
}

bool UInventoryBackground::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	CV(UInventoryDragDropOperation, Operation, InOperation);
	if (!IsValid(Operation))return true;
	CurrentItemWidget = Operation->GetItem();
	if (CurrentItemWidget->GetPickupInfo().Count > 1) {
		AmountInput->SetText(FText::AsNumber(CurrentItemWidget->GetPickupInfo().Count));
		DropAmountBorder->SetVisibility(ESlateVisibility::Visible);
		return true;
	}
	
	if (CurrentItemWidget->GetPickupInfo().Count == 1) 
		DragDropFromContainer(CurrentItemWidget->GetContainer()->GetContainerType(), CurrentItemWidget->GetUniqueServerID(), 1);
	CurrentItemWidget = nullptr;
	return true;
}

void UInventoryBackground::DragDropFromContainer(ESlotType _ContainerType, int32 _ServerID, int32 _Amount)
{
	if (IsValid(PlayerOwner)) {
		PlayerOwner->InventoryDropItemRequest(_ContainerType, _ServerID, _Amount);
	}
}
