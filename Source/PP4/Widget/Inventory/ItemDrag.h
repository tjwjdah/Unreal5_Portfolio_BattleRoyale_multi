// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ItemDrag.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UItemDrag : public UUserWidget
{
	GENERATED_BODY()
	
	private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UObject> Image;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D Size;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UImage> ItemImage;

	protected:
	virtual void NativeConstruct();

public:
	void SetImage(UObject* _Image) { Image = _Image; }
	void SetSize(FVector2D _Size) { Size = _Size; }

	void Init();
};
