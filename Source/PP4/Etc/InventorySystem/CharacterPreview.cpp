// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPreview.h"


#include "../../Items/PickupItem.h"

#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ACharacterPreview::ACharacterPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent= Scene;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);

	SkelBodyArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelBodyArmor"));
	SkelBodyArmor->SetupAttachment(Mesh);

	SkelHeadArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelHeadArmor"));
	SkelHeadArmor->SetupAttachment(Mesh);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void ACharacterPreview::BeginPlay()
{
	Super::BeginPlay();
	CV(ACharacter, Char, GetOwner());
	if (IsValid(Char)) {
		
		Mesh->SetSkeletalMesh(Char->GetMesh()->GetSkeletalMeshAsset());
		Mesh->SetMaterial(0, Char->GetMesh()->GetMaterial(0));
	}
	SceneCaptureComponent2D->ShowOnlyComponents.Add(SkelBodyArmor);
	SceneCaptureComponent2D->ShowOnlyComponents.Add(SkelHeadArmor);
	SceneCaptureComponent2D->Deactivate();
	
}

// Called every frame
void ACharacterPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterPreview::ClearAllSlots()
{
	SkelBodyArmor->SetSkeletalMesh(nullptr);
	SkelHeadArmor->SetSkeletalMesh(nullptr);
	if (IsValid(PrimaryAttached))
		PrimaryAttached->Destroy();
	if (IsValid(SecondaryAttached))
		SecondaryAttached->Destroy();
	if (IsValid(PistolAttached))
		PistolAttached->Destroy();
	if (IsValid(MeleeAttached))
		MeleeAttached->Destroy();
	if (IsValid(Backpack))
		Backpack->Destroy();
	SceneCaptureComponent2D->ShowOnlyActors.Empty();
}


void ACharacterPreview::UpdateClothSlot(ESlotType _Slot, const FCharacterClothInfo& _ClothInfo)
{
	USkinnedMeshComponent* TargetMesh=nullptr;
	switch (_Slot)
	{
	case ESlotType::HeadArmor:
		TargetMesh = SkelHeadArmor;
		break;
	case ESlotType::BodyArmor:
		TargetMesh = SkelBodyArmor;
		break;
		/*
	case ESlotType::ArmorEnd:
		break;
	case ESlotType::Backpack:
		break;
	case ESlotType::Consumable:
		break;
	case ESlotType::ClothHeadWear:
		break;
	case ESlotType::ClothFaceWear:
		break;
	case ESlotType::ClothEyeWear:
		break;
	case ESlotType::ClothTopWear:
		break;
	case ESlotType::ClothPants:
		break;
	case ESlotType::ClothGloves:
		break;
	case ESlotType::ClothBoots:
		break;
	*/
	}
	if (!IsValid(TargetMesh))return;

	if(IsValid(_ClothInfo.SkeletaAttach))
		//PrintViewport(5.f, FColor::Red, _ClothInfo.SkeletaAttach.GetName());
	TargetMesh->SetSkeletalMesh(_ClothInfo.IsEquip ? _ClothInfo.SkeletaAttach : nullptr);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	TargetMesh->AttachToComponent(Mesh, Rules, _ClothInfo.AttachToSocket);
	TargetMesh->SetMasterPoseComponent(_ClothInfo.IsEquip && _ClothInfo.ShouldFollowMeshAnimation ? Mesh : nullptr);
}

void ACharacterPreview::WeaponEquip(AActor* _Actor, ESlotType _Slot)
{
	
	FActorSpawnParameters param;
	APickupItem* Item;
	FTransform Trans;
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	switch (_Slot)
	{
	case ESlotType::Primary:
		if (IsValid(PrimaryAttached)) {
			int32 aaa = SceneCaptureComponent2D->ShowOnlyActors.Remove(PrimaryAttached);
			PrimaryAttached->Destroy();
		}
		if (!IsValid(_Actor)) return;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Trans.SetLocation(GetActorLocation());
		PrimaryAttached = Cast<AActor>(GetWorld()->SpawnActor<AActor>(_Actor->GetClass(), Trans, param));
		Item = Cast<APickupItem>(PrimaryAttached);
		Item->SetPickupStatus(EPickupStatus::Equiped);
		Item->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item->GetInfo().UnEquipSocket);
		SceneCaptureComponent2D->ShowOnlyActors.Add(Item);
		break;
	case ESlotType::Secondary:
		if (IsValid(SecondaryAttached)) {
			SceneCaptureComponent2D->ShowOnlyActors.Remove(SecondaryAttached);
			SecondaryAttached->Destroy();
		}
		if (!IsValid(_Actor)) return;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Trans.SetLocation(GetActorLocation());
		SecondaryAttached = Cast<AActor>(GetWorld()->SpawnActor<AActor>(_Actor->GetClass(), Trans, param));
		Item = Cast<APickupItem>(SecondaryAttached);
		Item->SetPickupStatus(EPickupStatus::Equiped);
		Item->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item->GetInfo().UnEquipSocket2);
		SceneCaptureComponent2D->ShowOnlyActors.Add(Item);
		break;
	case ESlotType::Pistol:
		if (IsValid(PistolAttached)) {
			SceneCaptureComponent2D->ShowOnlyActors.Remove(PistolAttached);
			PistolAttached->Destroy();
		}
		if (!IsValid(_Actor)) return;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Trans.SetLocation(GetActorLocation());
		PistolAttached = Cast<AActor>(GetWorld()->SpawnActor<AActor>(_Actor->GetClass(), Trans, param));
		Item = Cast<APickupItem>(PistolAttached);
		Item->SetPickupStatus(EPickupStatus::Equiped);
		Item->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item->GetInfo().UnEquipSocket);
		SceneCaptureComponent2D->ShowOnlyActors.Add(Item);
		break;
	case ESlotType::Melee:
		if (IsValid(MeleeAttached)) {
			SceneCaptureComponent2D->ShowOnlyActors.Remove(MeleeAttached);
			MeleeAttached->Destroy();
		}
		if (!IsValid(_Actor)) return;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Trans.SetLocation(GetActorLocation());
		MeleeAttached = Cast<AActor>(GetWorld()->SpawnActor<AActor>(_Actor->GetClass(), Trans, param));
		Item = Cast<APickupItem>(MeleeAttached);
		Item->SetPickupStatus(EPickupStatus::Equiped);
		Item->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item->GetInfo().UnEquipSocket);
		SceneCaptureComponent2D->ShowOnlyActors.Add(Item);
		break;
	}
	
}

void ACharacterPreview::PrimaySecondarSwitch()
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	if (IsValid(PrimaryAttached)) {
		CV(APickupItem, Item, PrimaryAttached);
		Item->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item->GetInfo().UnEquipSocket2);
	}
	if (IsValid(SecondaryAttached)) {
		CV(APickupItem, Item2, SecondaryAttached);
		Item2->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item2->GetInfo().UnEquipSocket);
	}
	Swap(PrimaryAttached, SecondaryAttached);
	
}

void ACharacterPreview::BackpackEquip(const FPickupInfos& _BackpackInfo)
{
	if (IsValid(Backpack)) {
		SceneCaptureComponent2D->ShowOnlyActors.Remove(Backpack);
		Backpack->Destroy();
	}
	if (!_BackpackInfo.PickupClass)return;
	FActorSpawnParameters param;
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform Trans; Trans.SetLocation(GetActorLocation());
	Backpack = Cast<AActor>(GetWorld()->SpawnActor<AActor>(_BackpackInfo.PickupClass, Trans, param));
	CV(APickupItem, Item, Backpack);
	Item->SetPickupStatus(EPickupStatus::Equiped);
	Item->GetMeshComponentRef()->AttachToComponent(Mesh, Rules, Item->GetInfo().UnEquipSocket);
	SceneCaptureComponent2D->ShowOnlyActors.Add(Backpack);
}

void ACharacterPreview::RotationCharacter(float _RotationValue)
{
	Mesh->AddRelativeRotation(FRotator(0.f, _RotationValue,0.f));
}

void ACharacterPreview::TogglePreview(bool _Active)
{
	if (_Active)
		SceneCaptureComponent2D->Activate();
	else
		SceneCaptureComponent2D->Deactivate();
}

