// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDrag.h"

#include "Components/Image.h"

void UItemDrag::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}


void UItemDrag::Init()
{
	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(Image); // 이미지 설정
	SlateBrush.ImageSize = Size;      // 이미지 크기 설정
	SlateBrush.DrawAs = ESlateBrushDrawType::Image; // 일반 이미지로 그리기
	ItemImage->SetBrush(SlateBrush);
}
