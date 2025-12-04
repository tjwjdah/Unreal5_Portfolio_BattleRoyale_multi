// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemsContainer.h"
#include "ItemHover.h"
#include "ItemWidget.h"

#include "../../Character/BRCharacter.h"

#include "Components/Border.h"
#include "Components/WrapBox.h"
#include "Engine/Texture2D.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/SlateBlueprintLibrary.h"

void UItemsContainer::NativePreConstruct()
{
    /*
    static ConstructorHelpers::FClassFinder<UItemHover>	ItemHover(TEXT("WidgetBlueprint'/Game/Widget/Inventory/BPItemHover.BPItemHover_C'"));

    if (ItemHover.Succeeded())
        HoverWidgetClass = ItemHover.Class;

    static ConstructorHelpers::FClassFinder<UItemWidget> ItemW(TEXT("WidgetBlueprint'/Game/Widget/Inventory/BPItemWidget.BPItemWidget_C'"));

    if (ItemW.Succeeded())
        ItemWidgetClass = ItemW.Class;
        */
    Super::NativePreConstruct();
    if (!bInit) 
        Init();
}


FReply UItemsContainer::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    /*
    if (!IsValid(HoverWidget))
        return FReply::Handled();
    FVector2D	WidgetPos = InMouseEvent.GetScreenSpacePosition();

    const FVector2D LocalPos = InGeometry.AbsoluteToLocal(WidgetPos);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 0.f, FColor::Green,
            FString::Printf(TEXT("UI Local x: %.1f  y: %.1f"), WidgetPos.X, WidgetPos.Y));
    }
    HoverWidget->SetPositionInViewport(WidgetPos);
   */
    return FReply::Handled();;
}


void UItemsContainer::ItemMouseButtonDown(FKey _MouseButton, EPickupType _Type, int32 _Amount, int32 _ServerId)
{
    if (IsValid(PlayerOwner) && PlayerOwner->IsLocallyControlled()) {
        PlayerOwner->InventoryUseItem(_MouseButton, _Type, _Amount, _ServerId);
    }
}


void UItemsContainer::UpdateSlot(const FPickupInfos& _PickupInfo, int32 _SlotIndex, ESlotType _Type, int32 _ServerID)
{
    if (_SlotIndex < Items->GetChildrenCount()) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(_SlotIndex));
        if (IsValid(ItemWidget))
            ItemWidget->UpdateInfo(_PickupInfo, _Type, _ServerID);
   }
}

bool UItemsContainer::UpdateItem(const FPickupInfos& _PickupInfo, int32 _SlotIndex, ESlotType _Type)
{
    for (int32 i = 0; i < Items->GetChildrenCount(); i++)
    {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))continue;
        
        if (ItemWidget->GetSlotIndex() != _SlotIndex)continue;
        ItemWidget->UpdateInfo(_PickupInfo, _Type, ItemWidget->GetUniqueServerID());
        return true;
    }
    return false;
}

FPickupInfos UItemsContainer::FindConsumableTotalInfo(EPickupType _Type, int32& _Total)
{
    int32 TotalCount=0;
    FPickupInfos Info;
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))continue;
        if (ItemWidget->GetPickupInfo().Type == _Type) {
            TotalCount+=ItemWidget->GetPickupInfo().Count;
            Info = ItemWidget->GetPickupInfo();
        }
    }
    Info.Count = TotalCount;
    return Info;
}

void UItemsContainer::ZeroCount()
{
    if (Items->GetChildrenCount() > 0)
    {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(0));
        if (!IsValid(ItemWidget))return;
        ItemWidget->ZeroCount();
    }
 
}

void UItemsContainer::Init()
{
    bInit = true;
    
  
    FPickupInfos Info; Info.InventoryImage = SlotImage; Info.Status = EPickupStatus::InInventory;
    for (int32 i = 0; i < MaxSlots; i++) {
        UItemWidget* ItemWidget = Cast<UItemWidget>(CreateWidget(GetWorld(), ItemWidgetClass));
        ItemWidget->SetSize(SlotSize);
        ItemWidget->SetPickupInfo(Info);
        ItemWidget->SetBackgroundColor(SlotBackgroundColor);
        ItemWidget->SetSlotIndex(i);

        ItemWidget->SetHotKey(Hotkey);
        ItemWidget->SetCanDrag(CanDrag);
        ItemWidget->SetUniqueServerID(-1);
        ItemWidget->Init();
        Items->AddChild(ItemWidget);
        ItemWidget->ForceUpdateImage(SlotImage);
        ItemWidget->SetContainer(this);
    }
    Items->SetInnerSlotPadding(SlotPadding);
    MainBorder->SetBrushColor(BackgroundColor.GetSpecifiedColor());
}

bool UItemsContainer::GetSlotItemTypeInfo(EPickupType _Type, FPickupInfos& _Info, int32& _Index, int32& _ServerID)
{
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))
            continue;
        if (ItemWidget->GetPickupInfo().Type == _Type) {
            _Info = ItemWidget->GetPickupInfo();
            _Index = i;
            _ServerID = ItemWidget->GetUniqueServerID();
            return true;
        }
    }
    return false;
}

void UItemsContainer::ClearSlot(int32 _Index)
{
    if (_Index < Items->GetChildrenCount())
    {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(_Index));
        if (!IsValid(ItemWidget))return;
        ItemWidget->HideArmorValue();
        FPickupInfos Info; Info.InventoryImage = SlotImage; Info.Status = EPickupStatus::InInventory;
        ItemWidget->UpdateInfo(Info,ESlotType::Empty,-1);
    }
}

void UItemsContainer::UpdateCountID(int32 _ID, int32 _NewCount)
{
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget) || ItemWidget->GetUniqueServerID() != _ID)
            continue;
        if (_NewCount != 0) {
            ItemWidget->UpdateCount(_NewCount);
            break;
        }
        FPickupInfos Info; Info.InventoryImage = SlotImage;
        ItemWidget->UpdateInfo(Info,ESlotType::Empty,-1);
        break;
    }
}


void UItemsContainer::AddNewItem(const FPickupInfos& _Info, ESlotType _Slot, int32 _SlotIndex, int32 _UniqueServerID)
{
    int32 EmptyslotIndex = -1;
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))continue;
        if (_Info.CanStack) {
            if (ItemWidget->GetPickupInfo().Type == _Info.Type && ItemWidget->GetSlotStatus() != ESlotType::Empty) {
                ItemWidget->AddCount(_Info.Count);
                ItemWidget->UpdateCount(ItemWidget->GetPickupInfo().Count);
                return;
            }
            
            if (EmptyslotIndex < 0 && ItemWidget->GetSlotStatus() == ESlotType::Empty)
            {
                EmptyslotIndex = i;
                continue;
            }
         
        }
        if (ItemWidget->GetSlotStatus() != ESlotType::Empty)
            continue;
        if ((_SlotIndex != -1 && _SlotIndex == ItemWidget->GetSlotIndex()) || _SlotIndex == -1)
        {
            ItemWidget->UpdateInfo(_Info, _Slot, _UniqueServerID);
            return;
        }
    }
    if (EmptyslotIndex == -1)return;
    CV(UItemWidget, ItemWidget, Items->GetChildAt(EmptyslotIndex));
    if (!IsValid(ItemWidget))return;
    ItemWidget->UpdateInfo(_Info, _Slot, _UniqueServerID);
}

void UItemsContainer::CreateHover()
{
    HoverWidget = Cast<UItemHover>(CreateWidget(GetWorld(), HoverWidgetClass));
    HoverWidget->AddToViewport();
}

bool UItemsContainer::IsInteractableConatiner(ESlotType _Type)
{
    if(AcceptDropContainerType.Num()<=0)
    return false;
    
    return AcceptDropContainerType.Contains(_Type);
}

UItemWidget* UItemsContainer::GetSlotIndexItemWidget(int32 _SlotIndex)
{
    if (_SlotIndex >= Items->GetChildrenCount())
        return nullptr;
    CV(UItemWidget, ItemWidget, Items->GetChildAt(_SlotIndex));
    if (IsValid(ItemWidget))
            return ItemWidget;
    
    return nullptr;
}


UItemWidget* UItemsContainer::GetIDItemWidget(int32 _ID)
{
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))continue;

        if (ItemWidget->GetUniqueServerID() == _ID) {
            return ItemWidget;
        }
    }
    return nullptr;
}

UItemWidget* UItemsContainer::GetSlotInfoItemType(EPickupType _Type)
{
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))continue;

        if (ItemWidget->GetPickupInfo().Type == _Type) {
            return ItemWidget;
        }
    }
    return nullptr;
}

UItemWidget* UItemsContainer::FindInventoryItemID(FName _Name)
{
    for (int32 i = 0; i < Items->GetChildrenCount(); i++) {
        CV(UItemWidget, ItemWidget, Items->GetChildAt(i));
        if (!IsValid(ItemWidget))continue;

        if (ItemWidget->GetPickupInfo().ID == _Name) {
            return ItemWidget;
        }
    }
    return nullptr;
}

void UItemsContainer::UpdateCountSlotIndex(int32 _SlotIndex, int32 _NewCount)
{
    if (_SlotIndex >= Items->GetChildrenCount())return;
    CV(UItemWidget, ItemWidget, Items->GetChildAt(_SlotIndex));
    if (IsValid(ItemWidget))
        ItemWidget->UpdateCount(_NewCount);
}

bool UItemsContainer::GetEquip()
{
    CV(UItemWidget, ItemWidget, Items->GetChildAt(0));
    if (!IsValid(ItemWidget))return false;

    return ItemWidget->GetPickupInfo().ID != FName("None");
}
