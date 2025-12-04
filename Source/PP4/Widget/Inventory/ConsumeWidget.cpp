// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumeWidget.h"

#include "../../Character/BRCharacter.h"

#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"

void UConsumeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ItemName->SetText(Name);
	FWidgetAnimationDynamicEvent MyAnimationEvent;
	MyAnimationEvent.BindDynamic(this, &UConsumeWidget::AnimationDone);
	BindToAnimationFinished(LoadingAnim, MyAnimationEvent);

	PlayAnimation(LoadingAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f / AnimationTime);
	
}


void UConsumeWidget::AnimationDone()
{
	if (IsValid(PlayerOwner))
		PlayerOwner->UseItem(UniqueServerID);
}
