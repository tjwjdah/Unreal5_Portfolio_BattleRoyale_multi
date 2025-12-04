// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos PickupInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ESlotType> SlotStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D Size;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText HotKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool CanDrag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UItemsContainer> Container;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 UniqueServerID;

	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemDrag> ItemDragClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UOverlay> ArmorOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> ArmorValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> HotKeyText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UBorder> MainBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class USizeBox> MainSizeBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> ShieldImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> TextCount;

protected:
	virtual void NativeConstruct();
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	void Init();

	const FPickupInfos& GetPickupInfo() { return PickupInfo; }

	void AddCount(int32 _Count) { PickupInfo.Count += _Count; }

	ESlotType GetSlotStatus() { return SlotStatus;}

	void UpdateCount(int32 _NewCount);

	void UpdateInfo(const FPickupInfos& _Info, ESlotType _Type, int32 _ServerID);

	void ForceUpdateImage(UTexture2D* _Image);

	UItemsContainer* GetContainer() { return Container; }

	void SetContainer(UItemsContainer* _Container) { Container = _Container; }

	void SwitchInfo(UItemWidget* _Item1, UItemWidget* _Item2, bool _SwitchSlotIndex);
	
	void UpdateArmorValue(float _ArmorValue);

	void HideArmorValue();

	int32 GetUniqueServerID() { return UniqueServerID; }

	void SetUniqueServerID(int32 _ID) {UniqueServerID = _ID; }
	void SetSize(FVector2d _Size) { Size = _Size; }
	void SetPickupInfo(FPickupInfos _PickupInfo) { PickupInfo = _PickupInfo; }
	void SetBackgroundColor(FSlateColor _BackgroundColor) { BackgroundColor = _BackgroundColor; }
	void SetSlotIndex(int32 _SlotIndex) { SlotIndex = _SlotIndex; }
	int32 GetSlotIndex() { return SlotIndex; }
	void SetHotKey(FText _Key) { HotKey = _Key; }
	void SetCanDrag(bool _Drag) { CanDrag = _Drag; }

	void ZeroCount();
};
