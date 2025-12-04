// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UItemWidget> Item;


public:
	void SetItem(UItemWidget* _Item) { Item = _Item; }

	UItemWidget* GetItem() { return Item; }
};
