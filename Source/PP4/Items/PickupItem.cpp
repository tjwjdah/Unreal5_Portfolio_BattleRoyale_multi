// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "../Etc/InventorySystem/PickupComponent.h"

#include "Net/UnrealNetwork.h"
// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(Box);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Box);
	PickupComponent = CreateDefaultSubobject<UPickupComponent>(TEXT("PickupComponent"));
	
	SetReplicateMovement(true);
	SetReplicates(true);
	PickupStatus = EPickupStatus::Dropped;
	NewCount = -1;
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	Box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);   // PickupChannel
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block);    // Explosion
	
	Box->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Box->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECR_Ignore);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

}

void APickupItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickupItem, NewCount);
	DOREPLIFETIME(APickupItem, PickupStatus);
}



// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	if (NewCount != -1) 
		PickupComponent->UpdateCount(NewCount);

	
	StaticDefTransform = StaticMesh->GetRelativeTransform();
	SkelDefTransform = SkeletalMesh->GetRelativeTransform();
	if (HasAuthority() && PickupStatus == EPickupStatus::Dropped) {
		Box->SetSimulatePhysics(true);
	}
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::OnRep_PickupStatus()
{
	UpdateStatus();
}


void APickupItem::SetCollisionT(ECollisionEnabled::Type _Type, bool _Hide, bool _SimulPhysics)
{
	Box->SetSimulatePhysics(_SimulPhysics);
	Box->SetCollisionEnabled(_Type);
	SetActorHiddenInGame(_Hide);
}

void APickupItem::PickupNotEquip()
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	
	auto* Ref = GetMeshComponentRef();
	Ref->AttachToComponent(Box, Rules);
	Ref->SetRelativeTransform(GetDefaultTransform());
}



FPickupInfos& APickupItem::GetInfo()
{
	return PickupComponent->GetPickupInfos();
}

FText APickupItem::GetNameAndCount()
{
	return PickupComponent->GetNamePlusCount();
	
}

bool APickupItem::SetPickupStatus(EPickupStatus _Status)
{
	if (PickupComponent->GetPickupInfos().Status == _Status)
		return false;
	if (HasAuthority()) {
		PickupStatus = _Status;
		UpdateStatus();
	}

	return true;
}

void APickupItem::UpdateStatus()
{
	if (PickupStatus == EPickupStatus::Dropped) {
		SetCollisionT(ECollisionEnabled::QueryAndPhysics, false, true);
		PickupNotEquip();
	}
	else {
		SetCollisionT(ECollisionEnabled::Type::NoCollision, PickupStatus == EPickupStatus::Hidden, false);
	}
	PickupComponent->UpdatePickupStatus(PickupStatus);

	if (PickupStatus == EPickupStatus::Equiped) {
		SkeletalMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		StaticMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void APickupItem::UpdateCount(int32 _NewCount)
{
	PickupComponent->UpdateCount(_NewCount);
}

USceneComponent* APickupItem::GetMeshComponentRef()
{
	if (IsValid(StaticMesh->GetStaticMesh()))
		return StaticMesh;
	else
		return SkeletalMesh;
}

void APickupItem::MeshComponenReset()
{
	FTransform Trans;
	auto* Ref = GetMeshComponentRef();
	Trans.SetScale3D(GetDefaultTransform().GetScale3D());
	Ref->SetRelativeTransform(Trans);
}


FTransform APickupItem::GetDefaultTransform()
{
	if (IsValid(StaticMesh->GetStaticMesh()))
	{
		return StaticDefTransform;
	}
	else
	{
		return SkelDefTransform;
	}
}

void APickupItem::SERVERSetNewCount_Implementation(int32 _Count)
{
	NewCount = _Count;
}
