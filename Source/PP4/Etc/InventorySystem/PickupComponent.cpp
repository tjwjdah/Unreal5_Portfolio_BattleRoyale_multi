// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

#include "../../Main/BRGameInstance.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	
	DoOnce = true;
}


// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!DoOnce)return;
	DoOnce = false;

	CV(UBRGameInstance, GameInst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (const FPickupInfos* Row = GameInst->GetItemData(PickupID))
	{
		PickupInfos = *Row;
		PickupInfos.ActorRef = GetOwner();
	}
	// Initialize();
	// ...
	
}


// Called every frame
void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPickupComponent::UpdateCount(int32 _Count)
{
	PickupInfos.Count = _Count;
}

/*void UPickupComponent::Initialize()
{
	if (!DoOnce)return;
	DoOnce = false;

	CV(UBRGameInstance, GameInst, UGameplayStatics::GetGameInstance(GetWorld()));
		if (const FPickupInfos* Row = GameInst->GetItemData(PickupID))
		{
			PickupInfos = *Row;
			PickupInfos.ActorRef = GetOwner();
		}
	
}*/

void UPickupComponent::UpdatePickupStatus(EPickupStatus _Status)
{
	PickupInfos.Status = _Status;
}

FText UPickupComponent::GetNamePlusCount()
{
	if (PickupInfos.Count > 1) {
		return FText::FromString(FString::Printf(TEXT("%s x %d"), *PickupInfos.Name.ToString(), PickupInfos.Count));
	}
	else {
		return PickupInfos.Name;
	}
	return FText();
}

