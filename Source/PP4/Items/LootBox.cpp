// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"
#include "PickupItem.h"
#include "../Main/BRGameInstance.h"

#include "Kismet/KismetMathLibrary.h"
// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALootBox::OnBeginInteract(AActor* _InteractingActor)
{
	RollItems();
	Destroy();

}

void ALootBox::RollItems()
{
	CV(UBRGameInstance, GameInst, UGameplayStatics::GetGameInstance(GetWorld()));
	for (int32 i = 0; i < PotentialItemsSpawn.Num(); i++) {
		int Roll = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);
		if (Roll <= PotentialItemsSpawn[i].SpawnChance0To100) {
			const FPickupInfos* Row = GameInst->GetItemData(PotentialItemsSpawn[i].ItemID);

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FVector Loc = GetActorLocation();
			Loc.X += UKismetMathLibrary::RandomFloatInRange(-150.f, 150.f);
			Loc.Y += UKismetMathLibrary::RandomFloatInRange(-150.f, 150.f);
			Loc.Z += 35.f;
			FRotator Rot(0.f,0.f, 0.f);
			APickupItem* Item = GetWorld()->SpawnActor<APickupItem>(Row->PickupClass,
			Loc, Rot, param);
			Item->UpdateCount(PotentialItemsSpawn[i].Count);
		}
	}
}

