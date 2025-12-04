// Fill out your copyright notice in the Description page of Project Settings.


#include "AirDropBox.h"

#include "../Main/BRGameInstance.h"
#include "../Items/PickupItem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AAirDropBox::AAirDropBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	ParachuteBP = CreateDefaultSubobject<UChildActorComponent>(TEXT("ParachuteBP"));
	ParachuteBP->SetupAttachment(StaticMesh);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->OnComponentHit.AddDynamic(this, &AAirDropBox::OnHit);
}

// Called when the game starts or when spawned
void AAirDropBox::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AAirDropBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAirDropBox::OnBeginInteract(AActor* _InteractingActor)
{
	RollItems();
	Destroy();
}

void AAirDropBox::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ParachuteBP->SetVisibility(false, false);
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->InitialSpeed = 0.f;
}

void AAirDropBox::RollItems()
{
	CV(UBRGameInstance, GameInst, GetGameInstance());

	if (!GameInst)return;

	for (int32 i = 0; i < PotentialItemsSpawn.Num(); i++) {
		float Roll = FMath::FRandRange(0.f, 100.f);
		if (Roll <= PotentialItemsSpawn[i].SpawnChance0To100) {
			

			if (const FPickupInfos* Row = GameInst->GetItemData(PotentialItemsSpawn[i].ItemID))
			{
				FActorSpawnParameters	param;
				FTransform Trans;
				FVector Min; FVector Max; StaticMesh->GetLocalBounds(Min, Max);
				Trans.SetLocation(GetActorLocation()+FVector(Max.X+FMath::FRandRange(-200.f,200.f),Max.Y+ FMath::FRandRange(-200.f, 200.f),35.f));
				Trans.SetRotation(FRotator(0.f, FMath::FRandRange(-360.f, 360.f), 0.f).Quaternion());
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				APickupItem* PickupItem = GetWorld()->SpawnActor<APickupItem>(Row->PickupClass, Trans, param);
				if (IsValid(PickupItem))
					PickupItem->UpdateCount(PotentialItemsSpawn[i].Count);
			}
		}
	}
}

