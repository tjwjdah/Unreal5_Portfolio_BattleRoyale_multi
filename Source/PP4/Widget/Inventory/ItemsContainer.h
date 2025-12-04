// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ItemsContainer.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_FourParams(FDragDropContainerDelegate, ESlotType,int32, ESlotType,int32);
UCLASS()
class PP4_API UItemsContainer : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRCharacter> PlayerOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlotType ContainerType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MaxSlots;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D SlotSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D SlotPadding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor SlotBackgroundColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> SlotImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText Hotkey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ESlotType> AcceptDropContainerType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool CanDrag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool AlwaysShowCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UWrapBox> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UBorder> MainBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UItemHover> HoverWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemHover> HoverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemWidget> ItemWidgetClass;

	FDragDropContainerDelegate DragDropContainerDelegate;
	
	bool bInit = false;
protected:
	virtual void NativePreConstruct();
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
public:

	void ItemMouseButtonDown(FKey _MouseButton, EPickupType _Type, int32 _Amount, int32 _ServerId);

	
	void UpdateSlot(const FPickupInfos& _PickupInfo, int32 _SlotIndex, ESlotType _Type, int32 _ServerID);

	bool UpdateItem(const FPickupInfos& _PickupInfo, int32 _SlotIndex, ESlotType _Type);


	FPickupInfos FindConsumableTotalInfo(EPickupType _Type,int32& _Total);

	
	bool GetSlotItemTypeInfo(EPickupType _Type,FPickupInfos& _Info, int32& _Index, int32& _ServerID);

	void SetPlayerOwner(ABRCharacter* _Owner) { PlayerOwner = _Owner; }
	
	void ClearSlot(int32 _Index);

	void UpdateCountID(int32 _ID, int32 _NewCount);

	void AddNewItem(const FPickupInfos& _Info, ESlotType _Slot, int32 _SlotIndex, int32 _UniqueServerID);

	bool GetAlwaysShowCount() { return AlwaysShowCount; }

	ESlotType GetContainerType() { return ContainerType; }


	UItemHover* GetHoverWidget() { return HoverWidget; }

	void CreateHover();
	void SetHoverWidget(UItemHover* _HoverWidget) { HoverWidget = _HoverWidget; }

	bool IsInteractableConatiner(ESlotType _Type);

	TArray<ESlotType>& GetAcceptDropContainerType() { return AcceptDropContainerType; }

	UItemWidget* GetSlotIndexItemWidget(int32 _SlotIndex);

	UItemWidget* GetIDItemWidget(int32 _ID);

	UItemWidget* GetSlotInfoItemType(EPickupType _Type);

	UItemWidget* FindInventoryItemID(FName _Name);

	void UpdateCountSlotIndex(int32 _SlotIndex, int32 _NewCount);

	bool GetEquip();

	void ZeroCount();

	void Init();

	template <typename T>
	void SetDragDropContainerDelegate(T* Obj, void(T::* Func)(ESlotType, int32, ESlotType, int32))
	{
		DragDropContainerDelegate.BindUObject(Obj, Func);
	}

	void DragDropContainerOverContainer(ESlotType _ContainerRecType, int32 _RecSlotIndex, ESlotType _ContainerSenderType, int32 _SenderSlotIndex) {
		if (DragDropContainerDelegate.IsBound())
			DragDropContainerDelegate.Execute(_ContainerRecType, _RecSlotIndex, _ContainerSenderType, _SenderSlotIndex);
	}

};
