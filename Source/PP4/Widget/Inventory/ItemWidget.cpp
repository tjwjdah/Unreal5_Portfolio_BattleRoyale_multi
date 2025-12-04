// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"
#include "ItemHover.h"
#include "ItemsContainer.h"
#include "ItemDrag.h"
#include "InventoryDragDropOperation.h"

#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

#include "Blueprint/WidgetBlueprintLibrary.h"


void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}



void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	if (SlotStatus == ESlotType::Empty)return;

	if (PickupInfo.Type == EPickupType::None)return;
	if (!IsValid(Container->GetHoverWidget()) || !Container->GetHoverWidget()->IsVisible())
	{
		Container->CreateHover();
	}
	
	
	Container->GetHoverWidget()->UpdateWeightText(FText::AsNumber(PickupInfo.Weight* PickupInfo.Count));
	Container->GetHoverWidget()->UpdateInfo(PickupInfo.Name, PickupInfo.Description, PickupInfo.InventoryImage);
	Container->GetHoverWidget()->SetParent(this);
	
	//Container->GetHoverWidget()->InitPos();

	Container->GetHoverWidget()->WidgetFadeIn();
	
	Container->GetHoverWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (!IsValid(Container->GetHoverWidget()))return;
	if (SlotStatus == ESlotType::Empty)return;

	if (Container->GetHoverWidget())
		Container->GetHoverWidget()->WidgetFadeOut();
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	Container->ItemMouseButtonDown(InMouseEvent.GetEffectingButton(), PickupInfo.Type, PickupInfo.Count, UniqueServerID);
	if (SlotStatus != ESlotType::Empty && CanDrag ) {
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	else
	return FReply::Handled();;
}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UItemDrag* ItemDrag = Cast<UItemDrag>(CreateWidget(GetWorld(), ItemDragClass));
	ItemDrag->SetImage(ItemImage->Brush.GetResourceObject());
	ItemDrag->SetSize(FVector2D(100.f, 100.f));

	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass());
	CV(UInventoryDragDropOperation, IDragOp, OutOperation);
	if (!IsValid(IDragOp)) return;

	IDragOp->DefaultDragVisual = ItemDrag;
	IDragOp->Pivot = EDragPivot::CenterCenter;
	IDragOp->SetItem(this);
	
	if (IsValid(Container->GetHoverWidget())&& Container->GetHoverWidget()->IsVisible()) {
		Container->GetHoverWidget()->SetVisibility(ESlateVisibility::Hidden);
		Container->GetHoverWidget()->WidgetFadeOut();
	}
}

bool UItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	CV(UInventoryDragDropOperation, IDragOp, InOperation);
	if (!IsValid(IDragOp))return false;

	if (IDragOp->GetItem() == this) return true;

	UItemWidget* DItem = IDragOp->GetItem();

	if (DItem->Container->GetContainerType() == Container->GetContainerType()) {
		if ((SlotStatus == ESlotType::Inventory && DItem->SlotStatus == ESlotType::Inventory) || (SlotStatus == ESlotType::Empty && DItem->SlotStatus == ESlotType::Inventory))
			SwitchInfo(DItem, this, false);
	}
	else {
		bool bCanSwap = ((DItem->SlotStatus != ESlotType::Empty && SlotStatus != ESlotType::Empty))
			|| (SlotStatus == ESlotType::Empty || DItem->SlotStatus == ESlotType::Empty);
		if (Container->IsInteractableConatiner(DItem->Container->GetContainerType()) && bCanSwap)
			Container->DragDropContainerOverContainer(Container->GetContainerType(), SlotIndex, DItem->Container->GetContainerType(), DItem->SlotIndex);
	}
	return true;
}



void UItemWidget::Init()
{
	
	ItemImage->SetBrushResourceObject(PickupInfo.InventoryImage);
	ItemImage->SetBrushSize(Size);
	ItemImage->SetVisibility(IsValid(ItemImage->GetBrush().GetResourceObject()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	TextCount->SetText(FText::AsNumber(PickupInfo.Count));
	TextCount->SetVisibility(PickupInfo.Count > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	MainBorder->SetBrushColor(BackgroundColor.GetSpecifiedColor());
	HotKeyText->SetText(HotKey);
	HotKeyText->SetVisibility((HotKey.ToString() == TEXT("None") || HotKey.ToString() == TEXT("none") || HotKey.IsEmpty()) ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

}

void UItemWidget::UpdateCount(int32 _NewCount)
{
	TextCount->SetText(FText::AsNumber(_NewCount));
	TextCount->SetVisibility(_NewCount > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	PickupInfo.Count = _NewCount;
}

void UItemWidget::UpdateInfo(const FPickupInfos& _Info, ESlotType _Type, int32 _ServerID)
{
	PickupInfo = _Info;
	SlotStatus = _Type;
	UniqueServerID = _ServerID;
	PickupInfo.UniqueID = UniqueServerID;
	ItemImage->SetBrushFromTexture(PickupInfo.InventoryImage);
	ItemImage->SetBrushSize(Size);
	ItemImage->SetVisibility(!IsValid(ItemImage->GetBrush().GetResourceObject()) && SlotStatus == ESlotType::Empty ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	TextCount->SetText(FText::AsNumber(PickupInfo.Count));
	TextCount->SetVisibility(PickupInfo.Count > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (Container->GetAlwaysShowCount())
		TextCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	MainBorder->SetBrushColor(BackgroundColor.GetSpecifiedColor());
	
}

void UItemWidget::ForceUpdateImage(UTexture2D* _Image)
{
	PickupInfo.InventoryImage = _Image;
	ItemImage->SetBrushFromTexture(PickupInfo.InventoryImage);
	ItemImage->SetBrushSize(Size);
}

void UItemWidget::SwitchInfo(UItemWidget* _Item1, UItemWidget* _Item2, bool _SwitchSlotIndex)
{
	int32 TempUnique = _Item1->UniqueServerID;
	UItemsContainer* TempContainer = _Item1->Container;
	FPickupInfos Info = _Item1->PickupInfo;
	ESlotType TempSlotType = _Item1->SlotStatus;
	_Item1->UpdateInfo(_Item2->PickupInfo, _Item2->SlotStatus, _Item2->UniqueServerID);
	_Item1->SetContainer(_Item2->Container);
	_Item2->SetContainer(TempContainer);
	_Item2->UpdateInfo(Info, TempSlotType, TempUnique);
	
	if(_SwitchSlotIndex)
		Swap(_Item1->SlotIndex, _Item2->SlotIndex);
}

void UItemWidget::UpdateArmorValue(float _ArmorValue)
{
	ArmorValue->SetText(FText::AsNumber(_ArmorValue));
	
	FLinearColor B = FLinearColor::Yellow;
	ArmorValue->SetColorAndOpacity(_ArmorValue == 0.f ? FLinearColor(1.f, 0.f, 0.f) : FLinearColor(1.f, 0.38f, 0.f));
	ArmorOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UItemWidget::HideArmorValue()
{
	ArmorOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UItemWidget::ZeroCount()
{
	TextCount->SetText(FText::AsNumber(0));
	PickupInfo.Count = 1;
}


