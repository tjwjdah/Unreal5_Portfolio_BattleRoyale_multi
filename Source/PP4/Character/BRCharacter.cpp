// Fill out your copyright notice in the Description page of Project Settings.


#include "BRCharacter.h"

#include "../Main/BRPlayerController.h"
#include "../Main/BRPlayerState.h"
#include "../Main/BRGameState.h"
#include "../Main/BRGameMode.h"
#include "../Main/BRGameInstance.h"
#include "../Main/PlaneGameMode.h"
#include "../Map/AirDropBox.h"

#include "Animation/BRAnim.h"

#include "../Etc/InventorySystem/CharacterPreview.h"
#include "../Etc/MiniMapActors/BigMapMarker.h"

#include "../Widget/InGameWidget.h"
#include "../Widget/TeammateIcon.h"

#include "../Widget/Inventory/InventoryWidget.h"
#include "../Widget/Inventory/ConsumeWidget.h"
#include "../Widget/Inventory/EquipInfoWidget.h"
#include "../Widget/Inventory/ItemWidget.h"
#include "../Widget/Inventory/ItemsContainer.h"
#include "../Widget/Inventory/InventoryBackground.h"

#include "../Widget/Minimap/OverviewMap.h"
#include "../Widget/Minimap/MinimapComponent.h"
#include "../Widget/Minimap/MinimapWidget.h"

#include "../Items/PickupItem.h"
#include "../Items/BackpackItem.h"
#include "../Items/WeaponItem.h"
#include "../Items/MeleeWeaponItem.h"
#include "../Items/ArmorItem.h"
#include "../Items/ClothItem.h"
#include "../Items/ExplosivesItem.h"
#include "../Items/DamageTypes/MeleeDamage.h"
#include "../Items/ConsumableItem.h"
#include "../Items/LootBox.h"

#include "../Map/AirDropBox.h"
#include "../Vehicles/Vehicle.h"

#include "Components/WidgetComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/PlayerState.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
ABRCharacter::ABRCharacter()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	CharacterMesh(TEXT("/Game/Assetss/7-Assets/Character/Mesh/SK_Mannequin"));

	
	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> PunchSoundCue = (TEXT("/Game/Assetss/7-Assets/Sounds/PunchSound_Cue"));
	if (PunchSoundCue.Succeeded())
	{
		PunchSound = PunchSoundCue.Object;
	}

	static ConstructorHelpers::FClassFinder<AActor> ParachuteBPClass(TEXT("/Game/Assetss/7-Assets/Other/Parachute/ParachuteBP"));
	if (ParachuteBPClass.Succeeded())
	{
		ParachuteClass = ParachuteBPClass.Class;
	}
	
	

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Equip(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/EquipMontage"));

	if (Equip.Succeeded())
		EquipMontage = Equip.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Reload(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/Reload"));

	if (Reload.Succeeded())
		ReloadMontage = Reload.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Punch(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/Melee/PunchMontage"));

	if (Punch.Succeeded())
		PunchMontage = Punch.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	MeleeAttack(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/Melee/MeleeAtkMontage"));

	if (MeleeAttack.Succeeded())
		MeleeAttackMontage = MeleeAttack.Object;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Drink(TEXT("/Game/Assetss/2-BRCharacters/Animation/DrinkMontage"));

	if (Drink.Succeeded())
		DrinkMontage = Drink.Object;
		
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	ThrowGrenade(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/Melee/GrenadeMontage"));

	if (ThrowGrenade.Succeeded())
		ThrowGrenadeMontage = ThrowGrenade.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Bandage(TEXT("/Game/Assetss/2-BRCharacters/Animation/BandageMontage"));

	if (Bandage.Succeeded())
		BandageMontage = Bandage.Object;
	
	RootComponent = GetCapsuleComponent();
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(GetMesh(),TEXT("head"));

	TeammateIconWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TeammateIcon"));
	TeammateIconWidget->SetupAttachment(GetMesh());

	PunchSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PunchSphere"));
	PunchSphere->SetupAttachment(GetMesh(), TEXT("hand_r"));

	BootsSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BootsSkel"));
	BootsSkelMesh->SetupAttachment(GetMesh());

	GlovesSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesSkel"));
	GlovesSkelMesh->SetupAttachment(GetMesh());

	PantsSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PantsSkel"));
	PantsSkelMesh->SetupAttachment(GetMesh());

	TopWearSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopWearSkel"));
	TopWearSkelMesh->SetupAttachment(GetMesh());

	EyeWearSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EyeWearSkel"));
	EyeWearSkelMesh->SetupAttachment(GetMesh());

	FaceWearSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceWearSkel"));
	FaceWearSkelMesh->SetupAttachment(GetMesh());

	HeadWearSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadWearSkel"));
	HeadWearSkelMesh->SetupAttachment(GetMesh());

	ArmorSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorSkel"));
	ArmorSkelMesh->SetupAttachment(GetMesh());

	HeadArmorSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadArmorSkel"));
	HeadArmorSkelMesh->SetupAttachment(GetMesh());

	NadeLoc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NadeLoc"));
	NadeLoc->SetupAttachment(GetCapsuleComponent());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,TEXT("SpringEndpoint"));

	PostProcessOutOfZone = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessOutOfZone"));
	PostProcessOutOfZone->SetupAttachment(GetCapsuleComponent());

	PickupTraceLength = 350.f;

	PickupSpawnDistance = 150.f;

	BackpackSocketName = TEXT("Backpack");

	MaxInventoryCapacity = 500.f;

	PunchTraceLen = 100.f;

	PunchDamage = 25.f;

	NormalFOV = 90.f;

	AimSpeed = 10.f;

	AimFOV = 45.f;

	ScopeFOV = 45.f;

	RepHealth = 100.f;

	ReviveDuration = 10.f;

	ParachuteGravityScale = 0.05f;

	InAirForwardForce = 700.f;
	InAirRightLForce = 700.f;

	MarkerColor = FLinearColor(1.0f, 0.0f, 1.0f);

	NormalSpeed = 550.f;

	WalkSpeed = 180.f;

	SprintSpeed = 900.f;

	WalkingState = EWalkingState::Walking;

	OnDeltaRotateInPlace = 70.f;

	UseRotateInPlace = true;

	EnablePostProcessing = true;


	CurrentMicThreshold = -5.0f;

	PlayerInfo.ActiveSlot = ESlotType::Inventory;
	bReplicates = true;
	SetReplicateMovement(true);
	bEquipMontageOn = true;
	bDrawDebug = true;
	bMeleeDelay = true;
	bPunchDelay = true;
	PunchSphere->OnComponentBeginOverlap.AddDynamic(this, &ABRCharacter::OnPunchBeginOverlap);

	bBeginPlay = true;

}

void ABRCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GetMesh()->SetTickGroup(ETickingGroup::TG_DuringPhysics);

	DynMat = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0));

	DynMat->SetVectorParameterValue(TEXT("BodyColor"), CharacterColor);
	GetMesh()->SetMaterial(0, DynMat);
	HeadArmorSkelMesh->SetMasterPoseComponent(GetMesh());
	ArmorSkelMesh->SetMasterPoseComponent(GetMesh());
}

// Called when the game starts or when spawned
void ABRCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ABRCharacter::OnTakeAnyDamageHandler);
	AnimInstance = Cast<UBRAnim>(GetMesh()->GetAnimInstance());

	if (!HasAuthority())
	{
		if (RepInVehicle)
			SetInVehicle(true);
		return;
	}


	if (bBeginPlay) {
		bBeginPlay = false;
		HandleBeginPlay();
	}

	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;
	UseRotateInPlace = true;
	bUseControllerRotationYaw = false;

}
void ABRCharacter::HandleBeginPlay()
{
	PlayerInfo.TeamID = TeamID;
	PlayerInfo.WaterPercent = 100.f;
	PlayerInfo.BoostPercent = 0.f;
	FollowCamera->FieldOfView = NormalFOV;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	CreatePreviewCharacter();
	if (IsLocallyControlled()) {
		MyInventory = Cast<UInventoryWidget>(CreateWidget(GetWorld(), MyInventoryClass));
		MyInventory->SetOwningPlayer(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		MyInventory->AddToViewport();
		MyInventory->SetVisibility(ESlateVisibility::Hidden);
		MyMinimap = Cast<UMinimapWidget>(CreateWidget(GetWorld(), MyMinimapClass));
		MyMinimap->GetMinimapComponent()->MinimapStartUpdate(this);
		MyMinimap->AddToViewport();
		OverviewMap = Cast<UOverviewMap>(CreateWidget(GetWorld(), OverviewMapClass));
		OverviewMap->GetMinimapComponent()->MinimapStartUpdate(this);
		OverviewMap->AddToViewport();
		OverviewMap->SetVisibility(ESlateVisibility::Hidden);
		MyInventory->UpdateInventoryWeight(CurrentInventoryCapacity, MaxInventoryCapacity);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRCharacter::UpdateMinimapAndTeammates, 0.5f, true);
		MinimapInitialized = true;
		CV(ABRPlayerController, Cont, GetController());
		if (IsValid(Cont) && IsValid(Cont->GetInGameWidget())) {
			InGameWidget = Cont->GetInGameWidget();
			FTimerHandle TimerHandle2;
			GetWorldTimerManager().SetTimer(TimerHandle2, this, &ABRCharacter::LookForPickups, 0.1f, true);
			BindInventoryevents();
			FTimerHandle TimerHandle3;
			GetWorldTimerManager().SetTimer(TimerHandle3, this, &ABRCharacter::WidgetSetPlayerID,1.f, false);
		}

		EquipInfoW = Cast<UEquipInfoWidget>(CreateWidget(GetWorld(), EquipInfoWClass));
		EquipInfoW->AddToViewport();
		
	}
}
void ABRCharacter::CreatePreviewCharacter()
{
	
	if (!IsValid(PrevCharacter)) {
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Trans; Trans.SetLocation(FVector(99999.f, 99999.f, 99999.f));
		PrevCharacter = GetWorld()->SpawnActorDeferred<ACharacterPreview>(PrevCharacterClass, FTransform(), nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		PrevCharacter->SetOwner(this);
		PrevCharacter->FinishSpawning(Trans);
	}
}

void ABRCharacter::EventOnDropConsumables(ESlotType _ContainerRecType, int32 _RecSlotIndex, ESlotType _ContainerSenderType, int32 _SenderSlotIndex)
{
	OnConsumableDroppedOverSlot(_SenderSlotIndex);
}

void ABRCharacter::OnReviveProgressDone()
{
	if (IsValid(RevivingPlayer) && RevivingPlayer->IsPlayerDown) {
		SERVEROnReviveDone(RevivingPlayer);
		RevivingPlayer = nullptr;
	}
}

void ABRCharacter::WidgetSetPlayerID()
{
	if (IsValid(GetPlayerState()))
	{
		CV(UBRGameInstance, GameInst, GetGameInstance());
		InGameWidget->SetPlayerID(GameInst->GetCustomCharName() == TEXT("") ? GetPlayerState()->GetPlayerName() : GameInst->GetCustomCharName());
	}
	else {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRCharacter::WidgetSetPlayerID,5.f, false);
	}
}
// Called every frame
void ABRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled()) {
		UCameraComponent* Camera = FollowCamera->IsActive() ? FollowCamera : FPCamera;
		Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, Aiming ? (UseScopeFOV ? ScopeFOV : AimFOV) : NormalFOV, DeltaTime, AimSpeed));
	}
	if (HasAuthority() || IsLocallyControlled())
		RepControl = GetControlRotation();
	bool bMovingButNotDriver = (PlayerInfo.Movement == EMovementState::Driving || PlayerInfo.Movement == EMovementState::Moving) && !IsVehicleDriver;
	FRotator Ro = (RepControl - GetActorRotation()).GetNormalized();
	AimPitch = bMovingButNotDriver ? Ro.Pitch : 0.f;
	AimYaw = bMovingButNotDriver ? Ro.Yaw : 0.f;
	if (UseRotateInPlace && (IsLocallyControlled() || HasAuthority())) {
		if (PlayerInfo.Movement == EMovementState::Parachuting || UKismetMathLibrary::VSizeXY(GetVelocity()) > 0.f) {
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}
		else {
			float YawDelta = (GetControlRotation() - GetActorRotation()).GetNormalized().Yaw;
			if (RotatingInPlace || abs(YawDelta) >= OnDeltaRotateInPlace) {
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
				if (FMath::IsNearlyEqual(YawDelta, 0.f, 2.f)) {
					ExtraRotationChecks += 1;
					if (ExtraRotationChecks > 3) {
						RotatingInPlace = false;
						ExtraRotationChecks = 0.f;
					}
					else
						RotatingInPlace = true;
				}
				else
					RotatingInPlace = true;
			}
			else {
				GetCharacterMovement()->bUseControllerDesiredRotation = false;
			}
		}
	}
	if (MeleeAttacking) {
		CV(APickupItem, Melee, PlayerInfo.MeleeWeapon);

		if(IsValid(Melee) ) {
			
			USceneComponent* MeshComponentRef = Melee->GetMeshComponentRef();

			if (IsValid(MeshComponentRef) && MeshComponentRef->DoesSocketExist(TEXT("Point1")) && MeshComponentRef->DoesSocketExist(TEXT("Point2"))) {
				FHitResult OutHit;
				FCollisionQueryParams CollisionParams;
				CollisionParams.AddIgnoredActor(this);
				CollisionParams.AddIgnoredActor(PlayerInfo.MeleeWeapon);
				bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, MeshComponentRef->GetSocketLocation(TEXT("Point1")), MeshComponentRef->GetSocketLocation(TEXT("Point2")),
					ECollisionChannel::ECC_Visibility,CollisionParams);
				
				if (bHit) {
					CV(ACharacter, Char, OutHit.GetActor());
					if(IsValid(Char) && bMeleeDelay) {
						bMeleeDelay = false;
						FTimerHandle TimerHandle;
						GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRCharacter::MeleeDelayOn, 0.8f, false);
						SERVERMeleeWeaponAtkConnect(Char, OutHit.Location);
					}
				}

			}

		}
	}

	if (PlayerInfo.Movement == EMovementState::Parachuting) {
		if (IsLocallyControlled() || HasAuthority()) {
			if (BtnRightDown || BtnLeftDown)
				GetCharacterMovement()->AddImpulse(UKismetMathLibrary::GetRightVector(GetActorRotation()) * (BtnRightDown ? 1.f : -1.f * InAirRightLForce));
			if (BtnFwdDown)
				GetCharacterMovement()->AddImpulse(UKismetMathLibrary::GetForwardVector(GetActorRotation()) * (GoingBackWards ? -1.f : 1.f * InAirForwardForce));
		}
	}
	if (IsValid(LocalPlayerRef)) {
		TeammateIconWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LocalPlayerRef->GetActorLocation()));
	}
	
	if (SetCameraSpect) {
		CameraBoom->SetWorldRotation(FMath::RInterpTo(CameraBoom->GetComponentRotation(), GetActorRotation(), DeltaTime, 25.f));
	}
	
	if (RepHealth != PlayerInfo.Health) {
		SERVERRepHealth(PlayerInfo.Health);
	}

}



// Called to bind functionality to input
void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//
	//PlayerInputComponent->BindAxis("LookUp", this, &ABRCharacter::LookUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABRCharacter::PressAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABRCharacter::ReleaseAttack);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABRCharacter::Reload);

	PlayerInputComponent->BindAction("PrimarySlot", IE_Pressed, this, &ABRCharacter::PrimarySlot);

	PlayerInputComponent->BindAction("SecondarySlot", IE_Pressed, this, &ABRCharacter::SecondarySlot);

	PlayerInputComponent->BindAction("PistoleSlot", IE_Pressed, this, &ABRCharacter::PistoleSlot);

	PlayerInputComponent->BindAction("MeleeSlot", IE_Pressed, this, &ABRCharacter::MeleeSlot);

	PlayerInputComponent->BindAction("ExplosiveSlot", IE_Pressed, this, &ABRCharacter::ExplosiveSlot);

	PlayerInputComponent->BindAction("ComsumablesSlot", IE_Pressed, this, &ABRCharacter::ComsumablesSlot);

	PlayerInputComponent->BindAction("Unequip", IE_Pressed, this, &ABRCharacter::Unequip);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABRCharacter::CharacterJump);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABRCharacter::PressAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABRCharacter::ReleaseAim);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABRCharacter::PressSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABRCharacter::ReleaseSprint);

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &ABRCharacter::PressWalk);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &ABRCharacter::ReleaseWalk);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABRCharacter::CharacterCrouch);

	PlayerInputComponent->BindAction("LeanLeft", IE_Pressed, this, &ABRCharacter::PressLeanLeft);
	PlayerInputComponent->BindAction("LeanLeft", IE_Released, this, &ABRCharacter::ReleaseLeanLeft);

	PlayerInputComponent->BindAction("LeanRight", IE_Pressed, this, &ABRCharacter::PressLeanRight);
	PlayerInputComponent->BindAction("LeanRight", IE_Released, this, &ABRCharacter::ReleaseLeanRight);

	
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ABRCharacter::Inventory);


	PlayerInputComponent->BindAction("ChangeCamera", IE_Pressed, this, &ABRCharacter::ChangeCamera);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABRCharacter::PressInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ABRCharacter::ReleaseInteract);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABRCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("Turn", this, &ABRCharacter::Turn);
	//PlayerInputComponent->BindAxis("LookUp", this, &ABRCharacter::LookUp);	
}

void ABRCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (JumpingOffPlane) {
		SERVERJumpingOffPlane(false);
		GetCharacterMovement()->AirControl = 0.2f;
		GetCharacterMovement()->GravityScale = 1.f;
	}
	else {
		if (HasAuthority() && abs(GetVelocity().Z) > 1000.f) {
			DamageTaken(GetFallDamage(GetVelocity().Z),TEXT("Fall damage"),NULL,false);
		}
	}
	if (IsValid(Parachute)) {
		Parachute->Destroy();
		SERVERParachuting(false);
		CameraBoom->SetRelativeLocation(CboomDefaultRelative);
		GetCharacterMovement()->GravityScale = 1.f;
	}
}

void ABRCharacter::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CV(ABRCharacter, Char, DamageCauser);
	if (IsValid(Char))
		DamageTaken(Damage, TEXT("Unkown(AnyDamage)"), Char, false);
}

void ABRCharacter::Destroyed()
{
	Super::Destroyed();
	if (HasAuthority()) {
		CV(ABRGameMode, GameMode, UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode  && GameMode->IsDeathMatch())
			GameMode->SpawnOnDeathMatch(ControllerRef, GetClass(), TeamID);
	}
	if (IsValid(PrevCharacter)) {
		PrevCharacter->ClearAllSlots();
		PrevCharacter->Destroy();
	}
	if (IsValid(MyInventory))
		MyInventory->RemoveFromParent();
}

void ABRCharacter::PressAttack()
{
	if(!IsPlayerDown && IsValid(MyInventory) && (!MyInventory->IsVisible() && !UsingItem))
	if (IsPlayerDown || !IsValid(MyInventory) || MyInventory->IsVisible() || UsingItem)return;
	FOnMontageEnded EndDelegate;
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	AWeaponItem* Gun;
	ESlotType Type = PlayerInfo.ActiveSlot;

	switch (PlayerInfo.ActiveSlot)
	{
	case ESlotType::Inventory:
		
		if (AnimInstance->Montage_IsPlaying(PunchMontage))break;
		Punching = true;
		SERVERPlayMontageToAll(PunchMontage);
		PunchSphere->SetGenerateOverlapEvents(true);
		AnimInstance->Montage_Play(PunchMontage);
		EndDelegate.BindUObject(this, &ABRCharacter::PunchEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, PunchMontage);
		break;
	case ESlotType::Primary:
	case ESlotType::Secondary:
	case ESlotType::Pistol:
		Gun = Cast<AWeaponItem>(Weapon);
		if (!Gun)break;
		if (!Gun->CanFire()) {
			Gun->PlayMagEmptySound();
			break;
		}
		IsFiring = true;
		StopAnimMontage(GetCurrentMontage());
		SERVERFireWeapon();
		if (!IsValid(Gun) || !IsFiring)break;
		Gun->PlayLoopSoundandEffect();
		WeaponFire(Gun, Gun->GetLoopFire());
		break;
	case ESlotType::Melee:
		if (AnimInstance->Montage_IsPlaying(MeleeAttackMontage))break;
		if (!IsValid(Weapon))break;
		MeleeAttacking = true;
		SERVERPlayMontageToAll(MeleeAttackMontage);
		AnimInstance->Montage_Play(MeleeAttackMontage);
		EndDelegate.BindUObject(this, &ABRCharacter::MeleeEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);
		break;
	case ESlotType::Explosives:
		if (TotalGrenades.FragGrenade <= 0 && TotalGrenades.StunGrenade <= 0 && TotalGrenades.SmokeGrenade <= 0)break;
		if (AnimInstance->Montage_IsPlaying(ThrowGrenadeMontage))break;
		SERVERPlayMontageToAll(ThrowGrenadeMontage);
		AnimInstance->Montage_Play(ThrowGrenadeMontage);
		break;
	}
	if (!IsFiring || WalkingState==EWalkingState::Walking)return;
	SetWalkingState(EWalkingState::Walking);
	SERVERSetWalkingState(EWalkingState::Walking);
	SERVERSetGoingBackWards(GoingBackWards);
}

void ABRCharacter::ReleaseAttack()
{
	if (WalkingState != EWalkingState::Sprinting && BtnSprintDown && !Aiming) {
		SetWalkingState(EWalkingState::Sprinting);
		SERVERSetWalkingState(EWalkingState::Sprinting);
	}
	else if (WalkingState != EWalkingState::Walking && BtnWalkDown && !Aiming) {
		SetWalkingState(EWalkingState::Walking);
		SERVERSetWalkingState(EWalkingState::Walking);
		SERVERSetGoingBackWards(GoingBackWards);
	}
	else if (WalkingState != EWalkingState::Normal && !Aiming && (!BtnWalkDown && !BtnSprintDown && !GoingBackWards)) {
		SetWalkingState(EWalkingState::Normal);
		SERVERSetWalkingState(EWalkingState::Normal);
	}
	if(!IsFiring) return;
	SERVERStopFiring();
	IsFiring = false;
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	CV(AWeaponItem, Item, Weapon);
	if (Item)
		Item->StopSoundandEffect(); 
}
void ABRCharacter::Reload()
{
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	CV(AWeaponItem, Item, Weapon);
	ifnv(Item)return;
	if (AnimInstance->Montage_IsPlaying(Item->GetReloadMontage()))return;
	if (Item->GetCanReload() && GetTotalAmmoCountAmmoType(Item->GetAmmoType())>0) {
		SetEnableScope(false);
		if (Aiming) {
			Aiming = false;
			SERVERSetAiming(false);
		}
		SERVERPlayMontageToAll(Item->GetReloadMontage());
		AnimInstance->Montage_Play(Item->GetReloadMontage());
	}
}
void ABRCharacter::PrimarySlot()
{
	if (!HasWeapon(ESlotType::Primary) || !bEquipMontageOn)return;
	bEquipMontageOn = false;
	SetEnableScope(false);
	AnimInstance->Montage_Stop(0.f,GetCurrentMontage());
	SERVERPlayMontageToAll(EquipMontage);
	ChangeSlot = ESlotType::Primary;
	AnimInstance->Montage_Play(EquipMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABRCharacter::EquipMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void ABRCharacter::SecondarySlot()
{
	if (!HasWeapon(ESlotType::Secondary) || !bEquipMontageOn)return;
	bEquipMontageOn = false;
	SetEnableScope(false);
	AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
	SERVERPlayMontageToAll(EquipMontage);
	ChangeSlot = ESlotType::Secondary;
	AnimInstance->Montage_Play(EquipMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABRCharacter::EquipMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void ABRCharacter::PistoleSlot()
{
	if (!HasWeapon(ESlotType::Pistol) || !bEquipMontageOn)return;
	bEquipMontageOn = false;
	SetEnableScope(false);
	AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
	SERVERPlayMontageToAll(EquipMontage);
	ChangeSlot = ESlotType::Pistol;
	AnimInstance->Montage_Play(EquipMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABRCharacter::EquipMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void ABRCharacter::MeleeSlot()
{
	if (!HasWeapon(ESlotType::Melee) || !bEquipMontageOn)return;
	bEquipMontageOn = false;
	SetEnableScope(false);
	AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
	SERVERPlayMontageToAll(EquipMontage);
	ChangeSlot = ESlotType::Melee;
	AnimInstance->Montage_Play(EquipMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABRCharacter::EquipMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void ABRCharacter::ExplosiveSlot()
{
	if (!HasWeapon(ESlotType::Explosives))
	{
		if (!bEquipMontageOn)
			return;
		if (PlayerInfo.ActiveSlot == ESlotType::Explosives)
			SwitchActiveExplosives();
		return;
	}
	SetEnableScope(false);
	AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
	SERVERPlayMontageToAll(EquipMontage);
	ChangeSlot = ESlotType::Explosives;
	AnimInstance->Montage_Play(EquipMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABRCharacter::EquipMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void ABRCharacter::ComsumablesSlot()
{
	if (CurrentConsumableSlot == EPickupType::None)return;	
	FPickupInfos Info; int32 Index; int32 ServerID;
	if (!MyInventory->GetContainer(ESlotType::Inventory)->GetSlotItemTypeInfo(CurrentConsumableSlot, Info, Index, ServerID))return;
	InventoryUseItem(EKeys::RightMouseButton, CurrentConsumableSlot, -25, ServerID);
}

void ABRCharacter::Unequip()
{
	if (PlayerInfo.ActiveSlot == ESlotType::Inventory || !bEquipMontageOn)return;
	bEquipMontageOn = false;
	SetEnableScope(false);
	AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
	SERVERPlayMontageToAll(EquipMontage);
	ChangeSlot = ESlotType::Inventory;
	AnimInstance->Montage_Play(EquipMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABRCharacter::EquipMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void ABRCharacter::CharacterJump()
{
	if (IsPlayerDown)return;

	if (PlayerInfo.Movement == EMovementState::Moving) {
		if (GetCharacterMovement()->IsCrouching()) {
			UnCrouch();
		}
		else {
			Jump();
		}
	}
	else
	{
		if (PlayerInfo.Movement == EMovementState::Parachuting)
			SERVERRequestOpenParachute();
	}
}

void ABRCharacter::PressAim()
{
	if(IsPlayerDown || !IsValid(MyInventory) || MyInventory->IsVisible())return;
	if (PlayerInfo.Alive && (PlayerInfo.ActiveSlot == ESlotType::Primary || PlayerInfo.ActiveSlot == ESlotType::Secondary || PlayerInfo.ActiveSlot == ESlotType::Pistol)) {
		Aiming = true;
		SERVERSetAiming(Aiming);
		SetEnableScope(true);
		if (WalkingState == EWalkingState::Walking)return;
		SetWalkingState(EWalkingState::Walking);
		SERVERSetWalkingState(EWalkingState::Walking);
		SERVERSetGoingBackWards(GoingBackWards);
	}
	else {
		Aiming = false;
		SERVERSetAiming(Aiming);
		SetEnableScope(false);
		if (WalkingState == EWalkingState::Walking && !GoingBackWards) {
			if (BtnSprintDown) {
				SetWalkingState(EWalkingState::Sprinting);
				SERVERSetWalkingState(EWalkingState::Sprinting);
			}
			else {
				if (BtnWalkDown)return;
				SetWalkingState(EWalkingState::Normal);
				SERVERSetWalkingState(EWalkingState::Normal);
			}
		}
	}

}

void ABRCharacter::ReleaseAim()
{
	UnAim();
}

void ABRCharacter::UnAim()
{
	if (Aiming) {
		SERVERSetAiming(false);
	}
	Aiming = false;
	SetEnableScope(false);
	if (WalkingState == EWalkingState::Walking && !GoingBackWards) {
		if (BtnSprintDown) {
			SetWalkingState(EWalkingState::Sprinting);
			SERVERSetWalkingState(EWalkingState::Sprinting);
		}
		else {
			if (BtnWalkDown)return;
			SetWalkingState(EWalkingState::Normal);
			SERVERSetWalkingState(EWalkingState::Normal);
		}
	}
}

void ABRCharacter::PressSprint()
{
	if (GetStrafing() || IsPlayerDown || GoingBackWards)return;

	if (!GetCharacterMovement()->IsCrouching() && !Aiming) {
		BtnSprintDown = true;
		if (WalkingState == EWalkingState::Sprinting)return;
		SetWalkingState(EWalkingState::Sprinting);
		SERVERSetWalkingState(EWalkingState::Sprinting);
	}

}

void ABRCharacter::ReleaseSprint()
{
	BtnSprintDown = false;
	if (WalkingState != EWalkingState::Sprinting)return;
	SetWalkingState(EWalkingState::Normal);
	SERVERSetWalkingState(EWalkingState::Normal);
}

void ABRCharacter::PressWalk()
{
	if (IsPlayerDown || GetCharacterMovement()->IsCrouching())return;
	BtnWalkDown = true;
	if (WalkingState != EWalkingState::Walking)return;
	SetWalkingState(EWalkingState::Walking);
	SERVERSetWalkingState(EWalkingState::Walking);
	SERVERSetGoingBackWards(GoingBackWards);
}

void ABRCharacter::ReleaseWalk()
{
	BtnWalkDown = false;
	if (WalkingState != EWalkingState::Walking || GoingBackWards)return;
	SetWalkingState(EWalkingState::Normal);
	SERVERSetWalkingState(EWalkingState::Normal);
}

void ABRCharacter::CharacterCrouch()
{
	if (PlayerInfo.Movement != EMovementState::Moving) return;
	if (GetCharacterMovement()->IsCrouching()) {
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void ABRCharacter::PressLeanLeft()
{
	RepLeaning = -1.f;
	SERVERSetLeaning(RepLeaning);
}

void ABRCharacter::ReleaseLeanLeft()
{
	RepLeaning = 0.f;
	SERVERSetLeaning(RepLeaning);
}

void ABRCharacter::PressLeanRight()
{
	RepLeaning = 1.f;
	SERVERSetLeaning(RepLeaning);
}

void ABRCharacter::ReleaseLeanRight()
{
	RepLeaning = 0.f;
	SERVERSetLeaning(RepLeaning);
}

void ABRCharacter::Map()
{
	ShowHideOverviewmap();
}
void ABRCharacter::ShowHideOverviewmap()
{
	if (!IsValid(OverviewMap))return;

	if (OverviewMap->IsVisible()) {
		OverviewMap->SetVisibility(ESlateVisibility::Hidden);
		if (!MyInventory->IsVisible()) {
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
		}

	}
	else {
		OverviewMap->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(OverviewMap->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	}
}
void ABRCharacter::ChangeCamera()
{
	if (!Aiming)return;

	if (!FPCamera->IsActive()) {
		FollowCamera->Deactivate();
		FPCamera->Activate();
		PlayerInfo.isThirdPerson = false;
		//SERVERUpdateCameraMode(false);
	}
	else {
		FPCamera->Deactivate();
		FollowCamera->Activate();
		PlayerInfo.isThirdPerson = true;
		//SERVERUpdateCameraMode(true);
	}
}

void ABRCharacter::PressInteract()
{
	if (PlayerInfo.Alive)
		InteractWithWorld();
}

void ABRCharacter::ReleaseInteract()
{
	ifv(RevivingPlayer) {
		InGameWidget->StopRevive();
		SERVERBeingRevived(RevivingPlayer,false);
		RevivingPlayer = nullptr;
	}
}

void ABRCharacter::Inventory()
{
	ToggleInventory();
}


void ABRCharacter::MoveForward(float Scale)
{
	FRotator Rot(0.f, GetControlRotation().Yaw, 0.f);

	AddMovementInput(UKismetMathLibrary::GetForwardVector(Rot), Scale);
	if (Scale < 0.f && !GoingBackWards)
	{
		GoingBackWards = true;
		SetWalkingState(EWalkingState::Walking);
		SERVERSetWalkingState(EWalkingState::Walking);
		SERVERSetGoingBackWards(GoingBackWards);
	}
	else {
		if (Scale > 0.f && GoingBackWards) {
			GoingBackWards = false;
			if (!Aiming) 
			{
				SetWalkingState(EWalkingState::Normal);
				SERVERSetWalkingState(EWalkingState::Normal);
			}
			SERVERSetGoingBackWards(GoingBackWards);
		}
		else {
			if (Scale == 0.f && GoingBackWards && !Aiming) {
				GoingBackWards = false;
				if (!BtnWalkDown) {
					SetWalkingState(EWalkingState::Normal);
					SERVERSetWalkingState(EWalkingState::Normal);
					SERVERSetGoingBackWards(GoingBackWards);
				}
			}
		}
	}
	if (!BtnFwdDown && Scale != 0.f) 
		BtnFwdDown = true;
	else 
		if (Scale == 0.f && BtnFwdDown)
			BtnFwdDown = false;
	SERVERSetBtnFwdDown(BtnFwdDown);
}

void ABRCharacter::MoveRight(float Scale)
{
	FRotator Rot(0.f, GetControlRotation().Yaw, 0.f);
	AddMovementInput(UKismetMathLibrary::GetRightVector(Rot), Scale);
	if ((Scale > 0.f && !BtnRightDown) || (Scale < 0.f && !BtnLeftDown)) {
		BtnRightDown = Scale > 0.f;
		BtnLeftDown = Scale < 0.f;
	}
	else {
		if (Scale == 0.f && (BtnRightDown || BtnLeftDown)) {
			BtnRightDown = false;
			BtnLeftDown = false;
		}
	}
	SERVERSetBtnRightDown(BtnRightDown, BtnLeftDown);
}

void ABRCharacter::PunchEnd(UAnimMontage* _Montage, bool _Interrupted)
{
	Punching = false;
	PunchSphere->SetGenerateOverlapEvents(false);
}
void ABRCharacter::MeleeEnd(UAnimMontage* _Montage, bool _Interrupted)
{
	MeleeAttacking = false;
}

void ABRCharacter::EquipMontageEnd(UAnimMontage* _Montage, bool _Interrupted)
{
	bEquipMontageOn = true;
}

void ABRCharacter::ConsumeInterrupted(UAnimMontage* _Montage, bool _Interrupted)
{
	if (IsValid(ItemUseWidget)) {
		if (!UsingItem) {
			ItemUseWidget->RemoveFromParent();
			ItemUseWidget = nullptr;
		}
	}
}


void ABRCharacter::OnPunchBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	if (!bPunchDelay)return;
	bPunchDelay = false;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRCharacter::PunchDelayOn, 0.5f, false);
	CV(ACharacter, Char, OtherActor);
	ifnv(Char)return;
	if (Char != this && Punching) {
		SERVERPunchConnect(Char,PunchSphere->GetComponentLocation());
	}
}

void ABRCharacter::LookForPickups()
{
	
	
	if (!PlayerInfo.Alive ||
		IsPlayerDown ||
		PlayerInfo.Movement != EMovementState::Moving)
	{
		if (PlayerInfo.Movement == EMovementState::InPlane || PlayerInfo.Movement == EMovementState::Parachuting)return;
		InGameWidget->HideBottomInfo();
		return;
	}

	FVector Start = PlayerInfo.isThirdPerson ? FollowCamera->GetComponentLocation() : FPCamera->GetComponentLocation();
	FVector End = (PlayerInfo.isThirdPerson ? FollowCamera->GetForwardVector() : FPCamera->GetForwardVector()) *
		(PlayerInfo.isThirdPerson ? (CameraBoom->TargetArmLength + PickupTraceLength) : PickupTraceLength);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = false;
	
	FHitResult OutHit;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, Start + End, ECC_GameTraceChannel3,CollisionParams);
	/*
	if (bDrawDebug)
	{
		FColor TraceColor = FColor::Red;
		FColor TraceHitColor = FColor::Green;
		float DrawTime = 0.1f;
		DrawDebugLine(GetWorld(), Start, Start + End, bHit ? TraceHitColor : TraceColor, false, DrawTime, 0, 1.0f);
	}
	*/
	if (!OutHit.bBlockingHit) {
		CurrentHighlightedItem = nullptr;
		InGameWidget->HideBottomInfo();
		return;
	}

	AActor* HitActor = OutHit.GetActor();
	
	APickupItem* Pickup = Cast<APickupItem>(HitActor);
	//CV(APickupItem, Pikcup, HitActor);
	if (Pickup) {
		
		if (HitActor == CurrentHighlightedItem) return;
		CurrentHighlightedItem = HitActor;
		
		FText Name = Pickup->GetNameAndCount();
		
		InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[F] Pickup")), Name, FLinearColor(1.0f, 1.0f, 1.0f), FLinearColor(1.0f, 0.535417f, 0.f), 0.f);
		
		return;
	}
	AVehicle* Vehicle = Cast<AVehicle>(OutHit.GetActor());

	if(Vehicle)
	{
		
		if (Vehicle->IsSeatEmpty()) {
			FName Name = FName(Vehicle->GetCarName());
			InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[F] Enter")), FText::FromName(Name), FLinearColor(1.0f, 1.0f, 1.0f), FLinearColor(1.0f, 0.535417f, 0.f), 0.f);
		}
		return;
	}
	ALootBox* LootBox = Cast<ALootBox>(HitActor);
	//CV(ALootBox, LootBox, HitActor);
	if (LootBox) {
		if (HitActor == CurrentHighlightedItem)return;
		CurrentHighlightedItem = HitActor;
		InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[F] Interact")), FText::FromString(TEXT("Loot box")), FLinearColor(1.0f, 1.0f, 1.0f), FLinearColor(1.0f, 0.535417f, 0.f), 0.f);
		return;
	}
	AAirDropBox* AirDropBox = Cast<AAirDropBox>(HitActor);
	if (AirDropBox) {
		if (HitActor == CurrentHighlightedItem)return;
		CurrentHighlightedItem = HitActor;
		InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[F] Interact")), FText::FromString(TEXT("Loot box")), FLinearColor(1.0f, 1.0f, 1.0f), FLinearColor(1.0f, 0.535417f, 0.f), 0.f);
		return;
	}
	ABRCharacter* Char = Cast<ABRCharacter>(HitActor);
	//CV(ABRCharacter, Char, HitActor);
	if (Char)
	{
		
		if (Char->IsPlayerDown) {
			if (!AddedTeammates.Contains(Char))return;
			CurrentHighlightedItem = Char;
			InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[F] Revive")), FText::FromString(TEXT("Teammate")), FLinearColor(1.0f, 0.0f, 0.0f), FLinearColor(1.0f, 1.f, 1.f), 0.f);
			return;
		}

	}

	CurrentHighlightedItem = nullptr;
	InGameWidget->HideBottomInfo();
}
void ABRCharacter::InteractWithWorld()
{
	
	if (!PlayerInfo.Alive || IsPlayerDown || PlayerInfo.Movement != EMovementState::Moving)
	{
		if (PlayerInfo.Alive && PlayerInfo.Movement == EMovementState::Driving)
			SERVERRequestExitVehicle();
		return;
	}

	FVector Start = PlayerInfo.isThirdPerson ? FollowCamera->GetComponentLocation() : FPCamera->GetComponentLocation();
	FVector End = (UKismetMathLibrary::GetForwardVector(GetBaseAimRotation())) *
		(PlayerInfo.isThirdPerson ? (CameraBoom->TargetArmLength + PickupTraceLength) : PickupTraceLength);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = false;
	FHitResult OutHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, Start + End, ECC_GameTraceChannel3,
		CollisionParams);
	/*
	if (bDrawDebug)
	{
		FColor TraceColor = FColor::Red;
		FColor TraceHitColor = FColor::Green;
		float DrawTime = 0.3f;
		DrawDebugLine(GetWorld(), Start, Start + End, bHit ? TraceHitColor : TraceColor, false, DrawTime, 0, 1.0f);
	}
	*/
	if (!OutHit.bBlockingHit || !IsValid(OutHit.GetActor()))return;

	AActor* HitActor = OutHit.GetActor();
	CV(APickupItem, Pikcup, HitActor);
	if (Pikcup)
	{
		SetEnableScope(false);

		auto& Info = Pikcup->GetInfo();
		if (Info.Status != EPickupStatus::Dropped)return;
		if (Info.Type == EPickupType::Backpack) {
			CV(ABackpackItem, Backpack, HitActor);
			if (!Backpack)return;
			if (PlayerInfo.BackpackLV <= Backpack->GetLevel()) {
				SERVERPickupBackPack(Backpack);
			}
			else {
				InGameWidget->ShowBottomInfo(FText::FromString(TEXT("You have a better one.")), FText::FromString(TEXT("")), FLinearColor(1.0f, 0.f, 0.f), FLinearColor(1.0f, 0.f, 0.f), 2.5f);
			}
		}
		else
		{
			float NewWeight;
			// 장착 가능하거나, 인벤토리에 들어갈 수 있으면 서버에 픽업 요청
			if (Info.CanEquiped || CanAddedInventory(Info.Count, Info.Weight, 0.f, NewWeight)) {
				SERVERPickupItem(HitActor);
			}
			else {
				InGameWidget->ShowBottomInfo(FText::FromString(TEXT("INVENTORY FULL")), FText::FromString(TEXT("")), FLinearColor(1.0f, 0.f, 0.f), FLinearColor(1.0f, 0.f, 0.f), 2.f);
			}
		}
		return;
	}
	CV(AVehicle, Vehicle, HitActor)
		if (Vehicle) {

		if (Vehicle->IsSeatEmpty() && !InVehicle()) {
			SERVERRequestEnterVehicle(HitActor);
			if (MyInventory->IsVisible())ToggleInventory();
		}
		return;
	}

	CV(ALootBox, LootBox, HitActor);
	if (LootBox) {
		SERVERRequestLootBoxInteract(HitActor);
		return;
	}
	CV(AAirDropBox, AirDropBox, HitActor);
	if (AirDropBox) {
		SERVERRequestLootBoxInteract(HitActor);
		return;
	}
	CV(ABRCharacter, Char, HitActor);
	if (Char && Char->IsPlayerDown && AddedTeammates.Contains(Char)) {
		InGameWidget->StartRevive(ReviveDuration, true);
		RevivingPlayer = Char;
		SERVERBeingRevived(RevivingPlayer, true);
		return;
	}
	
}

void ABRCharacter::CanAutoEquiped(TArray<TEnumAsByte<ESlotType>>& SupportedSlots, bool& _CanAuto, ESlotType& _Equip, bool& _IsCloth)
{
	//지원 슬롯을 기준으로, 예를 들어 주무기가 비어 있는데 방금 주무기를 주웠을 때 그 무기를 자동으로 장착할 수 있는지 확인
	_CanAuto = false;             // 기본값
	_Equip = ESlotType::Empty;
	_IsCloth = false;

	if (!IsValid(PlayerInfo.PrimaryWeapon) && SupportedSlots.Contains(ESlotType::Primary)) { _Equip = ESlotType::Primary, _CanAuto = true, _IsCloth = false; return; }
	if (!IsValid(PlayerInfo.SecondaryWeapon) && SupportedSlots.Contains(ESlotType::Secondary)) { _Equip = ESlotType::Secondary, _CanAuto = true, _IsCloth = false; return; }
	if (!IsValid(PlayerInfo.PistolWeapon) && SupportedSlots.Contains(ESlotType::Pistol)) { _Equip = ESlotType::Pistol, _CanAuto = true, _IsCloth = false; return; }
	if (!IsValid(PlayerInfo.MeleeWeapon) && SupportedSlots.Contains(ESlotType::Melee)) { _Equip = ESlotType::Melee, _CanAuto = true, _IsCloth = false; return; }

	if (!HeadArmorInfo.HasArmor && SupportedSlots.Contains(ESlotType::HeadArmor)) { _Equip = ESlotType::HeadArmor, _CanAuto = true, _IsCloth = true; return; }
	if (!BodyArmorInfo.HasArmor && SupportedSlots.Contains(ESlotType::BodyArmor)) { _Equip = ESlotType::BodyArmor, _CanAuto = true, _IsCloth = true; return; }

	if (!ClothHeadWearInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothHeadWear)) { _Equip = ESlotType::ClothHeadWear, _CanAuto = true, _IsCloth = true; return; }
	if (!ClothFaceWearInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothFaceWear)) { _Equip = ESlotType::ClothFaceWear, _CanAuto = true, _IsCloth = true; return; }
	if (!ClothEyeWearInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothEyeWear)) { _Equip = ESlotType::ClothEyeWear, _CanAuto = true, _IsCloth = true; return; }
	if (!ClothTopWearInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothTopWear)) { _Equip = ESlotType::ClothTopWear, _CanAuto = true, _IsCloth = true; return; }
	if (!ClothPantsInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothPants)) { _Equip = ESlotType::ClothPants, _CanAuto = true, _IsCloth = true; return; }
	if (!ClothGlovesInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothGloves)) { _Equip = ESlotType::ClothGloves, _CanAuto = true, _IsCloth = true; return; }
	if (!ClothBootsInfo.IsEquip && SupportedSlots.Contains(ESlotType::ClothBoots)) { _Equip = ESlotType::ClothBoots, _CanAuto = true, _IsCloth = true; return; }
	
	return;
}
int32 ABRCharacter::GetWeaponAmmoTotal()
{

	if (PlayerInfo.ActiveSlot == ESlotType::Primary) {
		CV(AWeaponItem, Weapon, PlayerInfo.PrimaryWeapon);
		if (!IsValid(Weapon))return -1;
		
		return GetTotalAmmoCountAmmoType(Weapon->GetAmmoType());
	}
	if (PlayerInfo.ActiveSlot == ESlotType::Secondary) {
		CV(AWeaponItem, Weapon, PlayerInfo.SecondaryWeapon);
		if (!IsValid(Weapon))return -1;

		return GetTotalAmmoCountAmmoType(Weapon->GetAmmoType());
		
	}

	if (PlayerInfo.ActiveSlot == ESlotType::Pistol) {
		CV(AWeaponItem, Weapon, PlayerInfo.PistolWeapon);
		if (!IsValid(Weapon))return -1;
	
		return  GetTotalAmmoCountAmmoType(Weapon->GetAmmoType());
		
	}
	if (PlayerInfo.ActiveSlot == ESlotType::Melee) {
		return -2;
	}

	if (PlayerInfo.ActiveSlot == ESlotType::Explosives) {
		return TotalGrenades.FragGrenade;;
	}
	return -1;
}
int32 ABRCharacter::GetWeaponAmmoCurrentMagSize()
{
	
	if (PlayerInfo.ActiveSlot == ESlotType::Primary) {
		CV(AWeaponItem, Weapon, PlayerInfo.PrimaryWeapon);
		if (!IsValid(Weapon))return -1;

		return Weapon->GetCurrentAmmo();
	}
	if (PlayerInfo.ActiveSlot == ESlotType::Secondary) {
		CV(AWeaponItem, Weapon, PlayerInfo.SecondaryWeapon);
		if (!IsValid(Weapon))return -1;

		return Weapon->GetCurrentAmmo();
	}

	if (PlayerInfo.ActiveSlot == ESlotType::Pistol) {
		CV(AWeaponItem, Weapon, PlayerInfo.PistolWeapon);
		if (!IsValid(Weapon))return -1;

		return Weapon->GetCurrentAmmo();
	}
	if (PlayerInfo.ActiveSlot == ESlotType::Melee) {
		return 0;
	}

	if (PlayerInfo.ActiveSlot == ESlotType::Explosives) {
		return 1;
	}
	return -1;
}
int32 ABRCharacter::GetTotalAmmoCountAmmoType(EAmmoType _AmmoType)
{
	return TotalAmmo.Types[static_cast<int8>(_AmmoType)];
}

UItemsContainer* ABRCharacter::GetContainerSlot(ESlotType _Slot)
{
	if (_Slot == ESlotType::Empty)return nullptr;

	UItemsContainer* Container = MyInventory->GetContainer(_Slot);
	switch (_Slot)
	{
	case ESlotType::Empty:
	case ESlotType::Backpack:
	case ESlotType::Consumable:
		Container = nullptr;
		break;
	}
	return Container;
}
UItemsContainer* ABRCharacter::GetInfoContainerSlot(ESlotType _Slot)
{
	if (_Slot == ESlotType::Empty)return nullptr;

	UItemsContainer* InfoContainer = nullptr;;
	switch (_Slot)
	{
	case ESlotType::Primary:
	case ESlotType::Secondary:
	case ESlotType::Pistol:
	case ESlotType::Melee:
	case ESlotType::Explosives:
	case ESlotType::HeadArmor:
	case ESlotType::BodyArmor:
		InfoContainer = EquipInfoW->GetContainer(_Slot);
		break;
	}
	return InfoContainer;
}
void ABRCharacter::ClientInventoryDragDrop(ESlotType _ContainerDropType, int32 _DropSlotIndex, ESlotType _ContainerDragType, int32 _DragSlotIndex)
{
	if ((_ContainerDropType == ESlotType::Primary && _ContainerDragType == ESlotType::Secondary) || 
		(_ContainerDropType == ESlotType::Secondary && _ContainerDragType == ESlotType::Primary)) {
		SERVERPrimarySecondarySwap();
		return;
	}

	
	ESlotType Type = _ContainerDropType == ESlotType::Inventory ? _ContainerDragType : _ContainerDropType;
	
	int32 SlotIndex = _ContainerDropType == ESlotType::Inventory ? _DropSlotIndex : _DragSlotIndex;
	AActor* Weapon = GetWeaponSlot(Type);
	//UItemsContainer* InventoryCont; UItemsContainer* InfoCont;
	//InventoryCont = GetContainerSlot(ESlotType::Inventory);
	if (!IsValid(Weapon)) {
		TArray<ESlotType> ClothContainerList = { ESlotType::BodyArmor,ESlotType::HeadArmor,ESlotType::ClothBoots,ESlotType::ClothEyeWear,ESlotType::ClothFaceWear,ESlotType::ClothGloves
		,ESlotType::ClothHeadWear,ESlotType::ClothPants,ESlotType::ClothTopWear};
		if ((ClothContainerList.Contains(_ContainerDragType) || ClothContainerList.Contains(_ContainerDropType)) && MyInventory->GetContainer(Type)->GetEquip()) 
		{
			UItemWidget* ItemWidget =MyInventory->GetContainer(Type)->GetSlotIndexItemWidget(0);
			float NewWeight;
			if (!CanAddedInventory(ItemWidget->GetPickupInfo().Count, ItemWidget->GetPickupInfo().Weight, 0, NewWeight)) {
				InGameWidget->ShowBottomInfo(FText::FromString(TEXT("INVENTORY FULL")), FText::FromString(TEXT("")), FLinearColor(1.0f, 0.f, 0.f), FLinearColor(1.0f, 0.f, 0.f), 3.f);
				return;
			}
		}
		else
		{
			//인벤토리에서 빈 슬롯(주 무기, 보조 무기 등)에 장착하기
			UItemWidget* ItemWidget = GetContainerSlot(ESlotType::Inventory)->GetSlotIndexItemWidget(_DragSlotIndex);
			SERVEREquipInventoryMainSlot(_ContainerDropType, ItemWidget->GetPickupInfo().UniqueID, _DragSlotIndex);
			return;
		}
	}
	else {
		CV(APickupItem, Pickup, Weapon); 
		if (!Pickup)return;
		auto& Info = Pickup->GetInfo();
		UItemWidget* ItemWidget = GetContainerSlot(ESlotType::Inventory)->GetSlotIndexItemWidget(SlotIndex);
		float NewWeight;
		if (!CanAddedInventory(Info.Count, Info.Weight, (ItemWidget ? ItemWidget->GetPickupInfo().Weight : 0), NewWeight)) {
			InGameWidget->ShowBottomInfo(FText::FromString(TEXT("INVENTORY FULL")), FText::FromString(TEXT("")), FLinearColor(1.0f, 0.f, 0.f), FLinearColor(1.0f, 0.f, 0.f), 2.f);
			return;
		}
	}
	UItemWidget* ItemWidget = GetContainerSlot(ESlotType::Inventory)->GetSlotIndexItemWidget(SlotIndex);
	SERVERDragDropEquipInventory(Type, ItemWidget->GetPickupInfo().UniqueID, SlotIndex);
}
void ABRCharacter::UpdateMinimapAndTeammates()
{
	CV(ABRPlayerController, PC, GetController());
	if (!PC)return;
	auto& Teammates = PC->GetTeammates();

	for (int32 i = 0; i < Teammates.Num(); i++) {
		if (!IsValid(Teammates[i])) continue;
		CV(ABRCharacter, Char, Teammates[i]);
		if (!Char)continue;

		if (AddedTeammates.Contains(Char)) {
			InGameWidget->UpdateTeammateHealth(Char->GetPlayerState()->GetPlayerId(), Char->RepHealth, Char->IsPlayerDown, Char->GetActorLocation());
			Char->SetupTeammateIcon(this);
		}
		else {
			//아직 추가되지 않았다면 팀원을 미니맵과 오버뷰 맵에 표시
			OverviewMap->GetMinimapComponent()->AddActor(Char, true, EMinimapImages::PositionIndic, FVector2D(50.f, 50.f), Char->MarkerColor, false, FText::FromString(TEXT("Teammate")));
			CV(ABRPlayerState, State, Char->GetPlayerState());
			FName name=*Char->GetPlayerState()->GetPlayerName();
			if (State)
				name = State->GetRepCustomName() == TEXT("") ? name : *State->GetRepCustomName();
			InGameWidget->AddTeammateHealthBar(Char->GetPlayerState()->GetPlayerId(), Char, name, Char->MarkerColor);
			AddedTeammates.Add(Char);
			i--;
		}
	}
}
void ABRCharacter::ChangeMinimapSettings(UTexture2D* _Texture, float _CapturedAreaSizeInWorldUnits, float _Zoom)
{
	MyMinimap->GetMinimapComponent()->ChangeMinimapTexture(_Texture, nullptr, _CapturedAreaSizeInWorldUnits, false, false);
	MyMinimap->GetMinimapComponent()->SetZoomValue(_Zoom);
	OverviewMap->GetMinimapComponent()->ChangeMinimapTexture(_Texture, nullptr, _CapturedAreaSizeInWorldUnits, false, false);
}
void ABRCharacter::RemoveActorOverviewmap(AActor* _Actor)
{
	OverviewMap->GetMinimapComponent()->RemoveActor(_Actor);
}
void ABRCharacter::EquipSocket(AActor* _Pickup, FName _SocketName, bool _Attache)
{
	CV(APickupItem, Pickup, _Pickup);
	if (!Pickup)return;
	Pickup->SetPickupStatus(EPickupStatus::Equiped);
	if (_Attache) {
		Pickup->MeshComponenReset();
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		_Pickup->AttachToComponent(GetMesh(), Rules, _SocketName);
	}

	CV(AWeaponItem, Weapon, _Pickup);
	if(Weapon)
		Weapon->SERVERSetWeaponOwner(this);
}
void ABRCharacter::ServerAttemptPickupItem(AActor* _Pickup)
{
	CV(APickupItem, Pickup, _Pickup);
	if (!Pickup)return;
	auto& Info = Pickup->GetInfo();
	if (Info.Status != EPickupStatus::Dropped)return; // 이미 주운 아이템 방지
	bool CanEquip; ESlotType EquipType; bool IsCloth;
	CanAutoEquiped(Info.SupportedSlots, CanEquip, EquipType, IsCloth);
	
	if (!CanEquip) {
		bool CanReplace; ESlotType SlotType;
		CanReplaceEquip(_Pickup, CanReplace, SlotType);
		if (CanReplace)
		{
			ServerReplaceEquip(_Pickup, SlotType);
			return;
		}
		float NewWeight; 
		if (!CanAddedInventory(Info.Count, Info.Weight, 0.f, NewWeight))
			return;
		if (Info.CanStack) {
			for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
				if (ServerInventoryItems[i].ID != Info.ID)continue;
				//추가된 항목이 있으면 갯수 업데이트
				ServerInventoryItems[i].Count += Info.Count;
				CurrentInventoryCapacity = NewWeight;
				CLIENTUpdateInventoryItemCount(ServerInventoryItems[i].ServerUniqueID, ServerInventoryItems[i].Count, CurrentInventoryCapacity);
				SERVERTotalAmmo(ServerInventoryItems[i].Type, ServerInventoryItems[i].Count);
				SERVERTotalGrenades(ServerInventoryItems[i].ID, ServerInventoryItems[i].Type, ServerInventoryItems[i].Count);
				if (Info.OnPickup == EOnPickupAction::None)
					Pickup->SetPickupStatus(EPickupStatus::InInventory);
				else if (Info.OnPickup == EOnPickupAction::Destroy)
					_Pickup->Destroy();
				else
					Pickup->SetPickupStatus(EPickupStatus::Hidden);
				return;
			}
		}
		CV(AConsumableItem, Cousumable, _Pickup);

		//픽업 아이템이 스택 가능한 경우라면, 인벤토리에서 해당 아이템을 찾을 수 없으므로 새 인스턴스를 만들어 인벤토리에 넣고,스택이 불가능한 경우에는 그대로 인벤토리에 추가

		FServerInventory Item;
		Item.ID = Info.ID;
		Item.ServerUniqueID = GetNewUniqueID();
		Item.Count = Info.Count;
		Item.Weight = Info.Weight;
		Item.Healing = Cousumable ? Cousumable->GetHealingValue() : 0.f;
		Item.PickupRef = _Pickup;
		Item.Type = Info.Type;
		Item.PickupClass = Info.PickupClass;
		Item.ArmorValue = 0.f;
		int32 Index = ServerInventoryItems.Add(Item);

		if (Info.OnPickup == EOnPickupAction::None)
			Pickup->SetPickupStatus(EPickupStatus::InInventory);
		else if (Info.OnPickup == EOnPickupAction::Destroy)
			_Pickup->Destroy();
		else
			Pickup->SetPickupStatus(EPickupStatus::Hidden);
		CurrentInventoryCapacity = NewWeight;
		// 소유자 클라이언트 인벤토리 UI 갱신
		CLIENTAddNewInventoryItem(ServerInventoryItems[Index].ID,
			ServerInventoryItems[Index].ServerUniqueID, _Pickup, CurrentInventoryCapacity, 
			-1, false, ServerInventoryItems[Index].Count);
		// 총 탄약/수류탄 수는 서버에서만 집계
		SERVERTotalAmmo(ServerInventoryItems[Index].Type, ServerInventoryItems[Index].Count);
		SERVERTotalGrenades(ServerInventoryItems[Index].ID, ServerInventoryItems[Index].Type,
			ServerInventoryItems[Index].Count);
		return;
	}

	//CanEquip 일때
	if (!IsCloth)
	{
		FName Socket = PlayerInfo.ActiveSlot == ESlotType::Inventory ?
			Info.EquipSocket : (EquipType == ESlotType::Secondary ? Info.UnEquipSocket2 : Info.UnEquipSocket);
		// NetMulticast 함수 모든 클라이언트에서 메쉬 부착/숨김 처리
		OnPickupEquipChanged(true, _Pickup, Socket, true, false);
	}

	if (static_cast<int8>(EquipType) >= static_cast<int8>(ESlotType::Primary) && static_cast<int8>(EquipType) < static_cast<int8>(ESlotType::WeaopnEnd))
	{
		SetWeaponRefSlot(EquipType, _Pickup);
		// 소유자 클라의 인벤토리/장비 UI 갱신
		CLIENTPickupEquip(Info.ID, EquipType, _Pickup, CurrentInventoryCapacity, -1, false);
		if (PlayerInfo.ActiveSlot != ESlotType::Inventory) return;
		PlayerInfo.ActiveSlot = EquipType;
		MCActiveSlotChanged(EquipType);
		return;
	}
	switch (EquipType)
	{
	case ESlotType::HeadArmor:
		{
		HeadArmorInfo.HasArmor = true;
		CV(AArmorItem, Armor, _Pickup);
		if (!Armor)return;
		HeadArmorInfo.ArmorValue = Armor->GetArmorValue();
		HeadArmorInfo.ArmorInfo = Info;
		HeadArmorInfo.ArmorSkelMesh = Armor->GetEquipSeletalMesh();
		CLIENTPickupEquip(Info.ID, EquipType, _Pickup, CurrentInventoryCapacity, -1, false);
		CLIENTUpdateArmorValue(EquipType, HeadArmorInfo.ArmorValue);
		FCharacterClothInfo ClothInfo;
		ClothInfo.IsEquip = true;
		ClothInfo.SkeletaAttach = HeadArmorInfo.ArmorSkelMesh;
		ClothInfo.ShouldFollowMeshAnimation = true;
		ClothInfo.AttachToSocket = TEXT("None");
		SERVERClothInfo(ESlotType::HeadArmor,ClothInfo);

		_Pickup->Destroy();
		return;
		}
	case ESlotType::BodyArmor:
		{
		BodyArmorInfo.HasArmor = true;
		CV(AArmorItem, Armor, _Pickup);
		if (!Armor)return;
		BodyArmorInfo.ArmorValue = Armor->GetArmorValue();
		BodyArmorInfo.ArmorInfo = Info;
		BodyArmorInfo.ArmorSkelMesh = Armor->GetEquipSeletalMesh();
		CLIENTPickupEquip(Info.ID, EquipType, _Pickup, CurrentInventoryCapacity, -1, false);
		CLIENTUpdateArmorValue(EquipType, BodyArmorInfo.ArmorValue);
		FCharacterClothInfo ClothInfo;
		ClothInfo.IsEquip = true;
		ClothInfo.SkeletaAttach = BodyArmorInfo.ArmorSkelMesh;
		ClothInfo.ShouldFollowMeshAnimation = true;
		ClothInfo.AttachToSocket = TEXT("None");
		SERVERClothInfo(ESlotType::BodyArmor, ClothInfo);
		_Pickup->Destroy();
		return;
		}
	}
	
	if (static_cast<int8>(EquipType)>= static_cast<int8>(ESlotType::ClothHeadWear) && static_cast<int8>(EquipType) < static_cast<int8>(ESlotType::ClothEnd))
	{
		CV(AClothItem, Cloth, _Pickup);
		FCharacterClothInfo ClothInfo;
		ClothInfo.IsEquip = true;
		ClothInfo.AttachToSocket = Cloth->GetAttachSocket();
		ClothInfo.ShouldFollowMeshAnimation = Cloth->GetShouldFollowMeshAnimation();
		ClothInfo.SkeletaAttach = Cloth->GetClothSeletalMesh();
		switch (EquipType)
		{
		case ESlotType::ClothHeadWear:
			ClothHeadData = Info;
			SERVERClothInfo(ESlotType::ClothHeadWear, ClothInfo);
			break;
		case ESlotType::ClothFaceWear:
			ClothFaceData = Info;
			SERVERClothInfo(ESlotType::ClothFaceWear, ClothInfo);
			break;
		case ESlotType::ClothEyeWear:
			ClothEyeData = Info;
			SERVERClothInfo(ESlotType::ClothEyeWear, ClothInfo);
			break;
		case ESlotType::ClothTopWear:
			ClothTopData = Info;
			SERVERClothInfo(ESlotType::ClothTopWear, ClothInfo);
			break;
		case ESlotType::ClothPants:
			ClothPantsData = Info;
			SERVERClothInfo(ESlotType::ClothPants, ClothInfo);
			break;
		case ESlotType::ClothGloves:
			ClothGlovesData = Info;
			SERVERClothInfo(ESlotType::ClothGloves, ClothInfo);
			break;
		case ESlotType::ClothBoots:
			ClothBootsData = Info;
			SERVERClothInfo(ESlotType::ClothBoots, ClothInfo);
			break;
		}
		CLIENTPickupEquip(Info.ID, EquipType, _Pickup, CurrentInventoryCapacity, -1, false);
		_Pickup->Destroy();
		return;
	}

}

void ABRCharacter::InventoryDropItemRequest(ESlotType _ContainerType, int32 _UniqueServerID, int32 _Amount)
{
	if (_ContainerType == ESlotType::Inventory)
		SERVERDropInventoryItem(_UniqueServerID, _Amount);
	else
		SERVERDropEquipItem(_ContainerType);
}
void ABRCharacter::ServerChangeSlot(ESlotType _Slot)
{
	if (_Slot == ESlotType::Inventory) {
		if (PlayerInfo.ActiveSlot == ESlotType::Inventory)return;
		AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
		if (!IsValid(Weapon))return;
		CV(APickupItem, Pickup, Weapon);
		if (!Pickup)return;
		auto& Info = Pickup->GetInfo();
		FName Name = PlayerInfo.ActiveSlot == ESlotType::Secondary ? Info.UnEquipSocket2 : Info.UnEquipSocket;
		OnPickupEquipChanged(true, Weapon, Name, true, false);
		PlayerInfo.ActiveSlot = ESlotType::Inventory;
		MCActiveSlotChanged(ESlotType::Inventory);
		return;
	}


	if (!HasWeapon(_Slot))
		return;


	AActor* Weapon = GetWeaponSlot(_Slot);
	if (IsValid(Weapon)) {
		CV(APickupItem, Pickup, Weapon);
		if (!Pickup)return;
		auto& Info = Pickup->GetInfo();
		OnPickupEquipChanged(true, Weapon, Info.EquipSocket, true, false);
	}
	else {
		if (!((PlayerInfo.ActiveSlot != ESlotType::Explosives) && (_Slot == ESlotType::Explosives) &&
			(TotalGrenades.FragGrenade > 0 || TotalGrenades.SmokeGrenade > 0 || TotalGrenades.StunGrenade > 0)))
			return;
		FName Name = TotalGrenades.FragGrenade > 0 ? TotalGrenades.FragGID : (TotalGrenades.SmokeGrenade > 0 ? TotalGrenades.SmokeGID : TotalGrenades.StunGID);
		CV(UBRGameInstance, GameInst, GetGameInstance());
		const FPickupInfos* Row = GameInst->GetItemData(Name);
		if (!Row)return;

		TSubclassOf<AExplosivesItem> ExplosivesItem;
		if (Row->PickupClass && Row->PickupClass->IsChildOf(AExplosivesItem::StaticClass()))
			ExplosivesItem = Row->PickupClass;
		if (!ExplosivesItem)return;
		AExplosivesItem* Explosives = GetWorld()->SpawnActorDeferred<AExplosivesItem>(ExplosivesItem, FTransform(), nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Explosives->SERVERSetNewCount(-1);
		Explosives->SetAttachSocketName(Row->EquipSocket);
		Explosives->FinishSpawning(FTransform());
		PlayerInfo.Explosive = Explosives;
		CurrentActiveExplosiveSlot= TotalGrenades.FragGrenade > 0 ? EPickupType::FragGrenade : 
			(TotalGrenades.SmokeGrenade > 0 ? EPickupType::SmokeGrenade : EPickupType::StunGrenade);
	}

	AActor* Weapon2 = GetWeaponSlot(PlayerInfo.ActiveSlot);
	if (IsValid(Weapon2)) {
		if (PlayerInfo.ActiveSlot == ESlotType::Explosives)
			Weapon2->Destroy();
		else {
			CV(APickupItem, Pickup2, Weapon2);
			if (!Pickup2)return;
			auto& Info2 = Pickup2->GetInfo();
			FName Name = PlayerInfo.ActiveSlot == ESlotType::Secondary ? Info2.UnEquipSocket2 : Info2.UnEquipSocket;
			OnPickupEquipChanged(true, Weapon2, Name, true, false);
		}
	}
	PlayerInfo.ActiveSlot = _Slot;
	MCActiveSlotChanged(_Slot);
}






AActor* ABRCharacter::GetWeaponSlot(ESlotType _Slot)
{
	switch (_Slot)
	{
	case ESlotType::Primary:
		return PlayerInfo.PrimaryWeapon;
	case ESlotType::Secondary:
		return PlayerInfo.SecondaryWeapon;
	case ESlotType::Pistol:
		return PlayerInfo.PistolWeapon;
	case ESlotType::Melee:
		return PlayerInfo.MeleeWeapon;
	case ESlotType::Explosives:
		return PlayerInfo.Explosive;
	}
	return nullptr;
}


void ABRCharacter::ServerDropInventoryItem(int32 _UniqueID, int32 _Count)
{
	bool AllDropped;
	int32 CurrentIndex;
	for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
		if (ServerInventoryItems[i].ServerUniqueID != _UniqueID) continue;
		if (ServerInventoryItems[i].Count < _Count)return;
		CurrentIndex = i;
		ServerInventoryItems[i].Count -= _Count;
		SERVERTotalAmmo(ServerInventoryItems[i].Type, ServerInventoryItems[i].Count);
		SERVERTotalGrenades(ServerInventoryItems[i].ID, ServerInventoryItems[i].Type, ServerInventoryItems[i].Count);
		SpawnPickup(ServerInventoryItems[i].PickupClass, ServerInventoryItems[i].Count);
		AllDropped = ServerInventoryItems[i].Count == 0;
		CurrentInventoryCapacity -= (ServerInventoryItems[i].Count * ServerInventoryItems[i].Weight);
		CLIENTUpdateInventoryItemCount(_UniqueID, ServerInventoryItems[i].Count, CurrentInventoryCapacity);
		break;
	}
	if (!AllDropped)return;
	//현재 수류탄을 장착한 상태에서 인벤토리의 모든 수류탄을 버리면 더 이상 수류탄을 사용할 수 없으므로 활성 슬롯을 None으로 설정
	ServerInventoryItems.RemoveAt(CurrentIndex);
	if (PlayerInfo.ActiveSlot != ESlotType::Explosives)return;
	if (IsValid(PlayerInfo.Explosive)) {
	PlayerInfo.Explosive->Destroy();
	}
	MCActiveSlotChanged(ESlotType::Inventory);
}

AActor* ABRCharacter::SpawnPickup(TSubclassOf<AActor> _PickupClass, int32 _Count)
{
	TSubclassOf<APickupItem> PickupClass;
	if (_PickupClass && _PickupClass->IsChildOf(APickupItem::StaticClass()))
		PickupClass = _PickupClass;
	if (!PickupClass)return nullptr;
	FTransform Trans;
	Trans.SetLocation(GetActorLocation() + (GetActorForwardVector() * PickupSpawnDistance));
	APickupItem* Pickup = GetWorld()->SpawnActorDeferred<APickupItem>(PickupClass, Trans, nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Pickup->SERVERSetNewCount(_Count);
	Pickup->FinishSpawning(Trans);
	return Pickup;
}


void ABRCharacter::ServerDropEquipItem(ESlotType _Slot)
{
	AActor* Weapon = GetWeaponSlot(_Slot);
	if (IsValid(Weapon)) {
		MCOnItemDrop(Weapon, GetPickupDropLocation(false));
	}
	else {
		if (_Slot == ESlotType::HeadArmor) {
			AActor* Pickup = SpawnPickup(HeadArmorInfo.ArmorInfo.PickupClass,-1);
			CV(AArmorItem, HeadArmor, Pickup);
			if (!HeadArmor)return;
			HeadArmor->SetArmorValue(HeadArmorInfo.ArmorValue);
			HeadArmorInfo.HasArmor = false;
			HeadArmorInfo.ArmorValue = 0.f;
			HeadArmorInfo.ArmorInfo = FPickupInfos();
			SERVERClothInfo(ESlotType::HeadArmor, FCharacterClothInfo());
		}
		else if (_Slot == ESlotType::BodyArmor)
		{
			AActor* Pickup = SpawnPickup(BodyArmorInfo.ArmorInfo.PickupClass, -1);
			CV(AArmorItem, Armor, Pickup);
			if (!Armor)return;
			Armor->SetArmorValue(BodyArmorInfo.ArmorValue);
			BodyArmorInfo.HasArmor = false;
			BodyArmorInfo.ArmorValue = 0.f;
			BodyArmorInfo.ArmorInfo = FPickupInfos();
			SERVERClothInfo(ESlotType::BodyArmor, FCharacterClothInfo());
		}
		else {
			FPickupInfos Info = *GetClothDataSlot(_Slot);
			AActor* Pickup = SpawnPickup(Info.PickupClass, -1);
			ClearClothSlot(_Slot);
		}
	}
	SetWeaponRefSlot(_Slot,nullptr);
	CLIENTEquipWeaponDrop(_Slot);
	if (PlayerInfo.ActiveSlot == _Slot) {
		PlayerInfo.ActiveSlot = ESlotType::Inventory;
		MCActiveSlotChanged(ESlotType::Inventory);
	}
	
	return;

}


void ABRCharacter::ServerEquipInventory(ESlotType _SlotType, int32 _UniqueID, int32 _SlotIndex)
{
	AActor* Weapon = GetWeaponSlot(_SlotType);
	FPickupInfos Info;
	APickupItem* Pickup =nullptr;
	bool Armor = (_SlotType == ESlotType::HeadArmor || _SlotType == ESlotType::BodyArmor);
	bool Cloth = ((static_cast<int8>(_SlotType) > static_cast<int8>(ESlotType::WeaopnEnd)) && (static_cast<int8>(_SlotType) < static_cast<int8>(ESlotType::ClothEnd)));
	if (IsValid(Weapon)) {
		Pickup = Cast<APickupItem>(Weapon);
		if (!Pickup)return;
		Info = Pickup->GetInfo();
	}
	else {
		if (!Cloth)return;
		Info = *GetClothDataSlot(_SlotType);
	}
	float NewWeigh;
	//_UniqueId ==-1 ToEmptySlot;
	if (!CanAddedInventory(Info.Count, Info.Weight, (_UniqueID == -1 ? 0.f : Info.Weight), NewWeigh))
		return;
	if (Cloth) {
		ClearClothSlot(_SlotType);
	}
	else {
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		Pickup->SetPickupStatus(EPickupStatus::Hidden);
	}
	FServerInventory ServerInventory;
	ServerInventory.ID = Info.ID;
	ServerInventory.ServerUniqueID = GetNewUniqueID();
	ServerInventory.Count = Info.Count;
	ServerInventory.Weight = Info.Weight;
	ServerInventory.Healing = 0.f;
	ServerInventory.PickupRef = Weapon;
	ServerInventory.Type = Info.Type;
	ServerInventory.PickupClass = Info.PickupClass;
	ServerInventory.ArmorValue = Armor ? (_SlotType == ESlotType::HeadArmor ? HeadArmorInfo.ArmorValue : BodyArmorInfo.ArmorValue) : 0;
	int32 index = ServerInventoryItems.Add(ServerInventory);
	CurrentInventoryCapacity = NewWeigh;
	CLIENTAddNewInventoryItem(ServerInventoryItems[index].ID, ServerInventoryItems[index].ServerUniqueID,
		ServerInventoryItems[index].PickupRef, CurrentInventoryCapacity, _SlotIndex, _UniqueID != -1, ServerInventoryItems[index].Count);
	if (_UniqueID == -1) {
		SetWeaponRefSlot(_SlotType,nullptr);
		if (PlayerInfo.ActiveSlot == _SlotType) {
			PlayerInfo.ActiveSlot = ESlotType::Inventory;
			MCActiveSlotChanged(ESlotType::Inventory);
		}
		CLIENTEquipWeaponDrop(_SlotType);
		if (!Armor)return;
		if (_SlotType == ESlotType::HeadArmor) {
			HeadArmorInfo.HasArmor = false; HeadArmorInfo.ArmorValue = 0.f; HeadArmorInfo.ArmorInfo = FPickupInfos();
		}
		else
		{
			BodyArmorInfo.HasArmor = false; BodyArmorInfo.ArmorValue = 0.f; BodyArmorInfo.ArmorInfo = FPickupInfos();
		}
		return;
	}
	if (Cloth)
	{
		if (Armor) {
			if (_SlotType == ESlotType::HeadArmor) {
				HeadArmorInfo.HasArmor = false; HeadArmorInfo.ArmorValue = 0.f; HeadArmorInfo.ArmorInfo = FPickupInfos();
			}
			else
			{
				BodyArmorInfo.HasArmor = false; BodyArmorInfo.ArmorValue = 0.f; BodyArmorInfo.ArmorInfo = FPickupInfos();
			}
		}
		for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
			if (ServerInventoryItems[i].ServerUniqueID != _UniqueID) continue;
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
			FTransform Trans; Trans.SetLocation(FVector(99999.f, 99999.f, 99999.f));
			APickupItem* PickupItem = Cast<APickupItem>(GetWorld()->SpawnActor<AActor>(ServerInventoryItems[i].PickupClass, Trans, param));
			CV(AArmorItem, ArmorItem, PickupItem);
			if (ArmorItem)
				ArmorItem->SetArmorValue(ServerInventoryItems[i].ArmorValue);
			ServerAttemptPickupItem(PickupItem);
			ServerInventoryItems.RemoveAt(i);
			return;
		}
	}
	else {
		bool removeitem;
		//인벤토리 또는 인벤토리에서 다른 무기 위로 무기를 드래그 앤 드롭
		for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
			index = i;
			if (ServerInventoryItems[i].ServerUniqueID != _UniqueID)continue;
			if (!IsValid(ServerInventoryItems[i].PickupRef)) break;
			CV(APickupItem, Pickup2, ServerInventoryItems[i].PickupRef);
			if (!Pickup2)return;
			auto& Info2 = Pickup2->GetInfo();
			if (!Info2.SupportedSlots.Contains(_SlotType))break;
			FName Name = PlayerInfo.ActiveSlot == _SlotType ? Info2.EquipSocket : (_SlotType == ESlotType::Secondary ? Info2.UnEquipSocket2 : Info2.UnEquipSocket);
			OnPickupEquipChanged(true, ServerInventoryItems[i].PickupRef, Name, true, false);
			CLIENTPickupEquip(Info2.ID, _SlotType, ServerInventoryItems[i].PickupRef, NewWeigh, -1, false);
			SetWeaponRefSlot(_SlotType, ServerInventoryItems[i].PickupRef);
			removeitem = true;
			break;
		}
		if (removeitem) {
			ServerInventoryItems.RemoveAt(index);
			return;
		}
		CLIENTEquipWeaponDrop(_SlotType);
		if (PlayerInfo.ActiveSlot != _SlotType)return;
		PlayerInfo.ActiveSlot = ESlotType::Inventory;
		MCActiveSlotChanged(ESlotType::Inventory);
	}
	


}
void ABRCharacter::SetWeaponRefSlot(ESlotType _Type, AActor* _Actor)
{
	switch (_Type)
	{
	case ESlotType::Primary:
		PlayerInfo.PrimaryWeapon = _Actor;
		break;
	case ESlotType::Secondary:
		PlayerInfo.SecondaryWeapon = _Actor;
		break;
	case ESlotType::Pistol:
		PlayerInfo.PistolWeapon = _Actor;
		break;
	case ESlotType::Melee:
		PlayerInfo.MeleeWeapon = _Actor;
		break;
	case ESlotType::Explosives:
		PlayerInfo.Explosive = _Actor;
		break;
	}
}
void ABRCharacter::UseItem(int32 _UniqueServerID)
{
	SERVERUseItem(_UniqueServerID);
	if (IsValid(ItemUseWidget))
	{
		ItemUseWidget->RemoveFromParent();
		ItemUseWidget = nullptr;
		UsingItem = false;
	}
}
void ABRCharacter::ServerSwapPrimarySecondary()
{
	AActor* PrimaryWeapon = GetWeaponSlot(ESlotType::Primary);
	AActor* SecondaryWeapon = GetWeaponSlot(ESlotType::Secondary);
	if (IsValid(PrimaryWeapon) && PlayerInfo.ActiveSlot != ESlotType::Primary) {
		CV(APickupItem, Pickup, PrimaryWeapon);
		if (Pickup)return;
		auto& PriInfo = Pickup->GetInfo();
		OnPickupEquipChanged(true, PrimaryWeapon, PriInfo.UnEquipSocket2, true, false);
	}
	if (IsValid(SecondaryWeapon) && PlayerInfo.ActiveSlot != ESlotType::Secondary) {
		CV(APickupItem, Pickup, SecondaryWeapon);
		if (Pickup)return;
		auto& SecondInfo = Pickup->GetInfo();
		OnPickupEquipChanged(true, SecondaryWeapon, SecondInfo.UnEquipSocket, true, false);
	}
	PlayerInfo.PrimaryWeapon = SecondaryWeapon; PlayerInfo.SecondaryWeapon = PrimaryWeapon;
	if (PlayerInfo.ActiveSlot == ESlotType::Primary) 
		PlayerInfo.ActiveSlot = ESlotType::Secondary;
	else if(PlayerInfo.ActiveSlot == ESlotType::Secondary)
		PlayerInfo.ActiveSlot = ESlotType::Primary;
	CLIENTPrimarySecondarySwapped(PlayerInfo.PrimaryWeapon, PlayerInfo.SecondaryWeapon, PlayerInfo.ActiveSlot);
	
}

void ABRCharacter::ServerEquipInventoryMainSlot(ESlotType _Slot, int32 _UniqueID, int32 _SlotIndex)
{
	//인벤토리에 있는 아이템을 비어 있는 슬롯(무기·방어구 등)으로 장착시키는 서버 RPC
	for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
		if (ServerInventoryItems[i].ServerUniqueID != _UniqueID)continue;

		if (IsValid(ServerInventoryItems[i].PickupRef)) {
		
			CV(APickupItem, Pickup, ServerInventoryItems[i].PickupRef);
			if (!Pickup)return;
			auto& Info = Pickup->GetInfo();
			if (!(!IsValid(GetWeaponSlot(_Slot)) && Info.SupportedSlots.Contains(_Slot)))return;

			FName Name = PlayerInfo.ActiveSlot == ESlotType::Inventory ?
				Info.EquipSocket : (_Slot == ESlotType::Secondary ? Info.UnEquipSocket2 : Info.UnEquipSocket);

			OnPickupEquipChanged(true, ServerInventoryItems[i].PickupRef, Name, true, false);
			CurrentInventoryCapacity -= Info.Weight;
			CLIENTPickupEquip(Info.ID, _Slot, ServerInventoryItems[i].PickupRef, CurrentInventoryCapacity, _SlotIndex, false);

			if (PlayerInfo.ActiveSlot == ESlotType::Inventory) {
				MCActiveSlotChanged(_Slot);
				PlayerInfo.ActiveSlot = _Slot;
			}

			SetWeaponRefSlot(_Slot, ServerInventoryItems[i].PickupRef);

		}
		else {

			bool Armor = ((static_cast<int8>(_Slot) > static_cast<int8>(ESlotType::WeaopnEnd)) && (static_cast<int8>(_Slot) < static_cast<int8>(ESlotType::ClothEnd)));
			if (!Armor || GetClothInfoSlot(_Slot).IsEquip)return;
			FActorSpawnParameters	param;
			FTransform Trans; Trans.SetLocation(FVector(99999.f, 99999.f, 99999.f));
			AActor* Item = GetWorld()->SpawnActor<AActor>(ServerInventoryItems[i].PickupClass, Trans, param);
			CV(AArmorItem, ArmorItem, Item);
			if (ArmorItem)ArmorItem->SetArmorValue(ServerInventoryItems[i].ArmorValue);
			ServerAttemptPickupItem(Item);
			CurrentInventoryCapacity -= ServerInventoryItems[i].Weight;
			CLIENTUpdateInventoryItemCount(_UniqueID, 0, CurrentInventoryCapacity);
		}
		ServerInventoryItems.RemoveAt(i);
		return;
	}

}

void ABRCharacter::CanReplaceEquip(AActor* _Pickup, bool& _CanReplace, ESlotType& _Slot)
{
	_CanReplace = false;
	_Slot = ESlotType::Inventory;
	CV(APickupItem, Pickup, _Pickup);
	if (!Pickup)return;
	auto& Info = Pickup->GetInfo();
	if (Info.SupportedSlots.Num() == 0)return;

	if (Info.SupportedSlots.Contains(ESlotType::Primary) && Info.SupportedSlots.Contains(ESlotType::Secondary)) {
		if (!IsValid(PlayerInfo.PrimaryWeapon) || !IsValid(PlayerInfo.SecondaryWeapon))return;
		if (PlayerInfo.ActiveSlot == ESlotType::Primary)
		{
			_CanReplace = true;
			_Slot = ESlotType::Primary;
		}
		else {
			_CanReplace = true;
			_Slot = ESlotType::Secondary;
		}
	}
	else {
		if (Info.SupportedSlots.Contains(ESlotType::Pistol)) {
			_CanReplace = true;
			_Slot = ESlotType::Pistol;
		}
		else if (Info.SupportedSlots.Contains(ESlotType::Melee)) {
			_CanReplace = true;
			_Slot = ESlotType::Melee;
		}
		else if (Info.SupportedSlots.Contains(ESlotType::HeadArmor)) {
			_CanReplace = true;
			_Slot = ESlotType::HeadArmor;
		}
		else if(Info.SupportedSlots.Contains(ESlotType::BodyArmor)){
			_CanReplace = true;
			_Slot = ESlotType::BodyArmor;
		}
	}
}
void ABRCharacter::DropPickupGround(AActor* _Pickup, FVector _NewLocation)
{
	CV(APickupItem, Pickup, _Pickup);
	if (!Pickup)return;
	_Pickup->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_Pickup->SetActorLocation(_NewLocation);
	Pickup->SetPickupStatus(EPickupStatus::Dropped);
}

void ABRCharacter::ServerReplaceEquip(AActor* _Pickup, ESlotType _Slot)
{
	//장착된 무기를 다른 무기로 교체하고 이전에 장착된 무기를 떨어뜨립니다
	// (예: 이미 권총을 가지고 있고 다른 권총을 선택했습니다). 장착된 권총을 떨어뜨리고 새 권총으로 교체합니다. 두 무기를 모두 보관하려면 먼저 장착된 권총을 인벤토리에 넣어야 합니다
	if (_Slot == ESlotType::HeadArmor || _Slot == ESlotType::BodyArmor)
	{
		AActor* Pickup= SpawnPickup(_Slot == ESlotType::HeadArmor ? HeadArmorInfo.ArmorInfo.PickupClass : BodyArmorInfo.ArmorInfo.PickupClass, -1);
		CV(AArmorItem, Armor, Pickup);
		Armor->SetArmorValue(_Slot == ESlotType::HeadArmor ? HeadArmorInfo.ArmorValue : BodyArmorInfo.ArmorValue);
		if (_Slot == ESlotType::HeadArmor) {
			HeadArmorInfo.HasArmor = false; HeadArmorInfo.ArmorValue = 0.f;
		}
		else {
			BodyArmorInfo.HasArmor = false; BodyArmorInfo.ArmorValue = 0.f;
		}
		ServerAttemptPickupItem(_Pickup);
	}
	else {
		AActor* Weapon = GetWeaponSlot(_Slot);
		if (!IsValid(Weapon))return;
		MCOnItemDrop(Weapon, GetPickupDropLocation(true));
		CV(APickupItem, Pickup, _Pickup);
		if (!Pickup)return;
		auto& Info = Pickup->GetInfo();
		FName Name = PlayerInfo.ActiveSlot == _Slot ?
			Info.EquipSocket : (_Slot == ESlotType::Secondary ? Info.UnEquipSocket2 : Info.UnEquipSocket);
		OnPickupEquipChanged(true, _Pickup, Name, true, false);
		CLIENTPickupEquip(Info.ID, _Slot, _Pickup, CurrentInventoryCapacity,-1,true);
		SetWeaponRefSlot(_Slot, _Pickup);
	}
}

FVector ABRCharacter::TraceToCrosshair(AActor* _Weapon, float _TraceLen)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(_Weapon);
	CollisionParams.AddIgnoredActor(VehicleRef);
	CollisionParams.bTraceComplex = false;
	FHitResult HitResult;
	FVector Start = PlayerInfo.isThirdPerson ? FollowCamera->GetComponentLocation() : FPCamera->GetComponentLocation();
	FVector End = (UKismetMathLibrary::GetForwardVector(GetBaseAimRotation())) *
		(PlayerInfo.isThirdPerson ? (CameraBoom->TargetArmLength + _TraceLen) : _TraceLen);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECollisionChannel::ECC_Visibility,
		CollisionParams);
	/*
	if (bDrawDebug)
	{
		FColor TraceColor = FColor::Red;
		FColor TraceHitColor = FColor::Green;
		float DrawTime = 0.3f;
		DrawDebugLine(GetWorld(), Start, Start + End, bHit ? TraceHitColor : TraceColor, false, DrawTime, 0, 1.0f);
	}
	*/
	if (HitResult.bBlockingHit)
		return HitResult.ImpactPoint;
	else
		return HitResult.TraceEnd;

}

void ABRCharacter::DamageTaken(float _Damage, FString _Name, ACharacter* _Player, bool _DamageProjectile)
{
	CV(ABRCharacter, Player, _Player);
	if (_Damage > 0 && HasAuthority()) {
		CV(ABRGameState, GameState, UGameplayStatics::GetGameState(GetWorld()));
		
		bool IsEnemy;
		if (IsValid(Player))
			IsEnemy = Player->TeamID != TeamID;
		else
			IsEnemy = true;
		if ((GameState->GetBRGameMode() == EGameModeList::Solo || IsEnemy || GameState->IsFriendlyFire()) &&
			(GameState->GetGameStatus() == EGameStatus::Started) && (PlayerInfo.Alive)) {
			float NewHealth = PlayerInfo.Health - _Damage > 0.f ? PlayerInfo.Health - _Damage : 0.f;
			if (NewHealth <= 0.f)
			{
				CV(ABRPlayerController, Cont, UGameplayStatics::GetPlayerController(GetWorld(), 0));
				if (Cont->HasTeammates() && !IsPlayerDown) {
					OnServerPlayerDown();
					return;
				}
			}
			
			PlayerInfo.Alive = (NewHealth>0.f);
			PlayerInfo.Health = NewHealth;
			SERVERRepHealth(PlayerInfo.Health);
			CLIENTUpdateHealth(PlayerInfo.Health, PlayerInfo.WaterPercent, PlayerInfo.BoostPercent);
			if (!PlayerInfo.Alive) {
				CV(ABRGameMode, GameMode, UGameplayStatics::GetGameMode(GetWorld()));
				if (GameMode)
					GameMode->PlayerDied(this, _Player,_Name);
				else {
					CV(APlaneGameMode, PlaneGameMode, UGameplayStatics::GetGameMode(GetWorld()));
					PlaneGameMode->PlayerDied(this, _Player, _Name);
				}
				GetMesh()->SetSimulatePhysics(true);
				MCClientYouDied();
				HandlePlayerDeath();
				SERVERDropEquipItemsOnDeath();
				if(IsValid(Player))
					Player->UpdateKillCount();
			}
			
		}
	}
	if (_Name != TEXT("Punch") && IsValid(Player) && _Player != this) {
		Player->CLIENTShowHitmarker();
	}

	
}


void ABRCharacter::ServerInventoryExplosiveUsed(FName _ID)
{
	int32 Count;
	for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
		if (ServerInventoryItems[i].ID != _ID)continue;
		Count = ServerInventoryItems[i].Count - 1;
		if (Count > -1) {
			CurrentInventoryCapacity -= ServerInventoryItems[i].Weight;
			CLIENTUpdateInventoryItemCount(ServerInventoryItems[i].ServerUniqueID, Count, CurrentInventoryCapacity);
			if (Count == 0) {
				ServerInventoryItems.RemoveAt(i);
			}
			else {
				ServerInventoryItems[i].Count = Count;
			}
			return;
		}
		break;
	}
	
}


void ABRCharacter::ServerUseConsumable(int32 _UniqueServerID)
{
	if (_UniqueServerID == -1 || !PlayerInfo.Alive)return;
	int32 Index;
	int32 Count;
	for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
		Index = i;
		if (ServerInventoryItems[i].ServerUniqueID != _UniqueServerID)continue;
		if (((ServerInventoryItems[i].Type == EPickupType::HealingItem1 || ServerInventoryItems[i].Type == EPickupType::HealingItem2) && PlayerInfo.Health < 100.f) ||
			(ServerInventoryItems[i].Type == EPickupType::HealingItem3 && PlayerInfo.WaterPercent < 100.f) ||
			(ServerInventoryItems[i].Type == EPickupType::HealingItem4 && PlayerInfo.BoostPercent < 100.f))
		{
			Count = ServerInventoryItems[i].Count - 1;
			if (ServerInventoryItems[i].Type == EPickupType::HealingItem1 || ServerInventoryItems[i].Type == EPickupType::HealingItem2) {
				PlayerInfo.Health = (ServerInventoryItems[i].Healing + PlayerInfo.Health) > 100.f ? 100.f : ServerInventoryItems[i].Healing + PlayerInfo.Health;
				SERVERRepHealth(PlayerInfo.Health);
			}
			else {
				if(ServerInventoryItems[i].Type==EPickupType::HealingItem3)
					PlayerInfo.WaterPercent = (ServerInventoryItems[i].Healing + PlayerInfo.WaterPercent) > 100.f ? 100.f : ServerInventoryItems[i].Healing + PlayerInfo.WaterPercent;
				else
					PlayerInfo.BoostPercent = (ServerInventoryItems[i].Healing + PlayerInfo.BoostPercent) > 100.f ? 100.f : ServerInventoryItems[i].Healing + PlayerInfo.BoostPercent;
			}
			CLIENTUpdateHealth(PlayerInfo.Health, PlayerInfo.WaterPercent, PlayerInfo.BoostPercent);
			break;
		}
		else
			return;
	}
	if (Count >= 0) {
		ServerInventoryItems[Index].Count = Count;
		CurrentInventoryCapacity -= ServerInventoryItems[Index].Weight;
		CLIENTUpdateInventoryItemCount(_UniqueServerID, Count, CurrentInventoryCapacity);
		if (Count == 0)
		{
			ServerInventoryItems.RemoveAt(Index);
			CLIENTConsumableClearSlot();
		}
	}
}

void ABRCharacter::ServerPickupBackpack(AActor* _Pickup)
{
	if (!IsValid(_Pickup))return;
	CV(ABackpackItem, Backpack, _Pickup);
	if (!Backpack)return;
	if (PlayerInfo.BackpackLV >= Backpack->GetLevel())return;
	auto& Info = Backpack->GetInfo();
	MaxInventoryCapacity = Backpack->GetCapacity();
	CLIENTBackPackEquip(MaxInventoryCapacity, Backpack->GetLevel(), Info);
	OnPickupEquipChanged(true, _Pickup, Info.EquipSocket, true, false);
	if (IsValid(PlayerInfo.BackpackRef)) {
		DropPickupGround(PlayerInfo.BackpackRef, GetPickupDropLocation(true));
	}
	PlayerInfo.BackpackRef = _Pickup;
}

void ABRCharacter::OnServerPlayerDown()
{
	PlayerInfo.Alive = true;
	PlayerInfo.Health = 100.f;
	SERVERIsPlayerDown(true);
	CLIENTUpdateHealth(PlayerInfo.Health, PlayerInfo.WaterPercent, PlayerInfo.BoostPercent);

	GetWorldTimerManager().SetTimer(PlayerDownHealthDepletedHandle, this, &ABRCharacter::PlayerDownHealthDepleted, 1.0f, true);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (PlayerInfo.Movement != EMovementState::Driving)return;
	MCPlayerExitedVehicle();
	CV(AVehicle, Vehicle, VehicleRef);
	Vehicle->SetSeatOwner(InVehicleSeat, nullptr);
}


EPickupType ABRCharacter::GetPickupTypeAmmoType(EAmmoType _AmmoType)
{
	switch (_AmmoType)
	{
	case EAmmoType::Type1:
		return EPickupType::Ammo;
	case EAmmoType::Type2:
		return EPickupType::Ammo2;
	case EAmmoType::Type3:
		return EPickupType::Ammo3;
	case EAmmoType::Type4:
		return EPickupType::Ammo4;
	case EAmmoType::Type5:
		return EPickupType::Ammo5;
	case EAmmoType::Type6:
		return EPickupType::Ammo6;
	case EAmmoType::Type7:
		return EPickupType::Ammo7;
	case EAmmoType::Type8:
		return EPickupType::Ammo8;
	}
	return EPickupType();
}

void ABRCharacter::SetInVehicle(bool _InVehicle)
{
	RepInVehicle = _InVehicle;
	PlayerInfo.Movement = RepInVehicle ? EMovementState::Driving : EMovementState::Moving;
	if (RepInVehicle) {
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->GravityScale = 0.f;
		GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
		//이 변수는 네트워크에서 클라이언트 이동 보정(에러 체크 + 위치 수정)을 무시할지 여부를 결정하는 플래그
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking, 0);
		GetCharacterMovement()->GravityScale = 1.f;
		GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = false;
	}
}

void ABRCharacter::UpdateKillCount()
{
	SERVERKillCount(KillCount + 1);
}

void ABRCharacter::PlayerDownHealthDepleted()
{
	if (!IsPlayerDown) {
		GetWorldTimerManager().ClearTimer(PlayerDownHealthDepletedHandle);
		return;
	}
	if (BeingRevived)return;
	DamageTaken(2.f, TEXT("Down"), DownedPlayer, false);
}

void ABRCharacter::WeaponFire(AWeaponItem* _Weapon, bool _Loop)
{
	if (!IsValid(_Weapon) || !IsFiring)return;
	_Weapon->PlayCamShake();
	if (_Weapon->CanFire())
	{
		if (!IsListenServer())
			_Weapon->SetCurrentMagSize(_Weapon->GetCurrentMagSize() - 1);
		InGameWidget->UpdateAmmoCount(_Weapon->GetCurrentMagSize(), GetTotalAmmoCountAmmoType(_Weapon->GetAmmoType()));
		if (_Loop && _Weapon->GetCurrentMagSize() > 0) {
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(
				TimerHandle, [this, _Weapon, _Loop]() {WeaponFire(_Weapon, _Loop); }, _Weapon->GetFireRate(), false);
			return;
		}
	}
	if (!IsFiring)return;
	_Weapon->StopSoundandEffect();
	IsFiring = false;
	SERVERStopFiring();
	return;
}

void ABRCharacter::SetWalkingState(EWalkingState _State)
{
	WalkingState = _State;
	switch (_State)
	{
	case EWalkingState::Walking:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EWalkingState::Normal:
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		break;
	case EWalkingState::Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;
	}
}

void ABRCharacter::AddReplicatedMarker(FSlateColor _Color, AActor* _Actor)
{
	if (IsValid(OverviewMap))
		OverviewMap->GetMinimapComponent()->AddActor(_Actor, false, EMinimapImages::LocationPointer, FVector2d(50.f, 50.f), _Color, false, FText::FromString(TEXT("Teammate marker")));
}

void ABRCharacter::InventoryUseItem(FKey _MouseButton, EPickupType _Type, int32 _Amount, int32 _ServerId)
{
	if ( (_Amount == -25 || _MouseButton == EKeys::RightMouseButton) &&
		(((_Type == EPickupType::HealingItem1 || _Type == EPickupType::HealingItem2) && PlayerInfo.Health < 100.f) ||
		  (_Type == EPickupType::HealingItem3 && PlayerInfo.WaterPercent < 100.f) ||
		  (_Type == EPickupType::HealingItem4 && PlayerInfo.BoostPercent < 100.f)) )
	{
		UsingItem = true;
		if (IsValid(ItemUseWidget)) {
			ItemUseWidget->RemoveFromParent();
		}
		auto Info = GetContainerSlot(ESlotType::Inventory)->GetIDItemWidget(_ServerId)->GetPickupInfo();
		ItemUseWidget = Cast<UConsumeWidget>(CreateWidget(GetWorld(), ItemUseWidgetClass));
		ItemUseWidget->SetAnimationTime(1.f);
		ItemUseWidget->SetUniqueServerID(_ServerId);
		ItemUseWidget->SetPlayerOwner(this);
		ItemUseWidget->SetItemName(Info.Name);
		ItemUseWidget->AddToViewport();
		AnimInstance->Montage_Play(_Type == EPickupType::HealingItem1 ? BandageMontage : DrinkMontage);
		SERVERPlayMontageToAll(_Type == EPickupType::HealingItem1 ? BandageMontage : DrinkMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ABRCharacter::ConsumeInterrupted);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, _Type == EPickupType::HealingItem1 ? BandageMontage : DrinkMontage);
	}
}



void ABRCharacter::HandleWaterDepletionAndBoost()
{
	if (!HasAuthority())return;
	if (PlayerInfo.Alive) {
		if (PlayerInfo.Movement != EMovementState::Moving) return;
		float NewWater = PlayerInfo.WaterPercent - (WalkingState == EWalkingState::Sprinting ? 0.5f :
			WalkingState == EWalkingState::Normal ? 0.25f : UKismetMathLibrary::VSizeXY(GetVelocity()) > 0 ? 0.1 : 0.f);
		if (NewWater <= 0)NewWater =0.f;
		PlayerInfo.WaterPercent = NewWater;
		if (PlayerInfo.WaterPercent <= 0.f) 
			DamageTaken(1.f, TEXT("Lack of Water"), nullptr, false);
		PlayerInfo.BoostPercent -= 1;
		if (PlayerInfo.BoostPercent <= 0.f) {
			PlayerInfo.BoostPercent = 0.f;
		}
		else {
			PlayerInfo.Health = PlayerInfo.Health < 100.f ? PlayerInfo.Health + 1 : 100.f;
		}
		CLIENTUpdateHealth(PlayerInfo.Health, PlayerInfo.WaterPercent, PlayerInfo.BoostPercent);
	}
	else {
		GetWorldTimerManager().ClearTimer(HandleWaterDepletionAndBoostHandle);
	}
}


void ABRCharacter::SERVERDropEquipItemsOnDeath()
{
	if (!HasAuthority())return;

	TArray<EPickupType> ItemsInventoryDrop = { EPickupType::Ammo,EPickupType::Ammo2,EPickupType::Ammo3,EPickupType::Ammo4,EPickupType::Ammo5,
		EPickupType::FragGrenade,EPickupType::SmokeGrenade,EPickupType::StunGrenade,
	EPickupType::HealingItem1,EPickupType::HealingItem2,EPickupType::HealingItem3,EPickupType::HealingItem4,EPickupType::HealingItem5 };

	if(IsValid(PlayerInfo.PrimaryWeapon))
		DropPickupGround(PlayerInfo.PrimaryWeapon, GetPickupDropLocation(true));

	if (IsValid(PlayerInfo.SecondaryWeapon))
		DropPickupGround(PlayerInfo.SecondaryWeapon, GetPickupDropLocation(true));

	if (IsValid(PlayerInfo.PistolWeapon))
		DropPickupGround(PlayerInfo.PistolWeapon, GetPickupDropLocation(true));

	if (IsValid(PlayerInfo.MeleeWeapon))
		DropPickupGround(PlayerInfo.MeleeWeapon, GetPickupDropLocation(true));

	for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
		if (!ItemsInventoryDrop.Contains(ServerInventoryItems[i].Type))continue;
		TSubclassOf<APickupItem> PickupClass;
		if (ServerInventoryItems[i].PickupClass && ServerInventoryItems[i].PickupClass->IsChildOf(APickupItem::StaticClass()))
			PickupClass = ServerInventoryItems[i].PickupClass;
		if (!PickupClass)return;
		FTransform Trans; Trans.SetLocation(GetPickupDropLocation(true));
		APickupItem* PickupItem = GetWorld()->SpawnActorDeferred<APickupItem>(PickupClass, Trans, nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		PickupItem->SERVERSetNewCount(ServerInventoryItems[i].Count);
		PickupItem->FinishSpawning(Trans);
	
	}
}

void ABRCharacter::OffPlaneTraceGround()
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = false;
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = FVector(0.f, 0.f, -15000.f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECollisionChannel::ECC_Visibility,
		CollisionParams);
	/*
	if (bDrawDebug)
	{
		FColor TraceColor = FColor::Red;
		FColor TraceHitColor = FColor::Green;
		float DrawTime = 3.0f;
		DrawDebugLine(GetWorld(), Start, Start + End, bHit ? TraceHitColor : TraceColor, false, DrawTime, 0, 1.0f);
	}*/
	if (!bHit)return;
	CV(ABRCharacter, Char, HitResult.GetActor());
	if (Char)return;
	GetWorldTimerManager().ClearTimer(OffPlaneTraceGroundHandle);
	if (!Parachuting && JumpingOffPlane)
		SERVERRequestOpenParachute();
}



void ABRCharacter::SetEnableScope(bool _Enable)
{
	
	if (_Enable) {
		AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
		if (!Weapon) return;

		CV(AWeaponItem, Gun, Weapon);
		float ScopeF = 0.f;
		TSubclassOf<UUserWidget> NewWidgetClass = Gun->GetScopeWidget(ScopeF);

		if (IsValid(ScropWidget) && ScropWidget->GetClass() != NewWidgetClass) {
			ScropWidget->RemoveFromParent();
			ScropWidget = nullptr;
		}
		if (!IsValid(ScropWidget) && IsValid(NewWidgetClass)) {
			ScropWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), NewWidgetClass));
			if (IsValid(ScropWidget))ScropWidget->AddToViewport();
		}
		if (ScropWidget) {
			ScropWidget->SetVisibility(ESlateVisibility::Visible);
			UseScopeFOV = true;
			ScopeFOV = ScopeF;
			GetCapsuleComponent()->SetVisibility(false, true);

		}
	}
	else {
		if (IsValid(ScropWidget))ScropWidget->SetVisibility(ESlateVisibility::Hidden);
		GetCapsuleComponent()->SetVisibility(true, true);
		UseScopeFOV = false;
	}
}


void ABRCharacter::ToggleInventory()
{

	if (!IsValid(MyInventory))return;

	if (MyInventory->IsVisible()) {
		MyInventory->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
		UWidgetBlueprintLibrary::CancelDragDrop();
	}
	else {
		MyInventory->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MyInventory->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	}
}

void ABRCharacter::ClearAllCloth()
{
	SERVERClothInfo(ESlotType::HeadArmor, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::BodyArmor, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothHeadWear, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothFaceWear, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothEyeWear, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothTopWear, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothPants, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothGloves, FCharacterClothInfo());
	SERVERClothInfo(ESlotType::ClothBoots, FCharacterClothInfo());


}

void ABRCharacter::SwitchActiveExplosives()
{
	EPickupType InitSlot = CurrentActiveExplosiveSlot;
	if (CurrentActiveExplosiveSlot == EPickupType::FragGrenade)
	{
		if (TotalGrenades.SmokeGrenade > 0)
			CurrentActiveExplosiveSlot = EPickupType::SmokeGrenade;
		else if(TotalGrenades.StunGrenade>0)
			CurrentActiveExplosiveSlot = EPickupType::StunGrenade;
	}
	else if(CurrentActiveExplosiveSlot == EPickupType::SmokeGrenade){
		if(TotalGrenades.StunGrenade>0)
			CurrentActiveExplosiveSlot = EPickupType::StunGrenade;
		else if (TotalGrenades.FragGrenade>0)
			CurrentActiveExplosiveSlot = EPickupType::FragGrenade;
	}
	else if (CurrentActiveExplosiveSlot == EPickupType::StunGrenade) {
		if (TotalGrenades.FragGrenade > 0)
			CurrentActiveExplosiveSlot = EPickupType::FragGrenade;
		else if (TotalGrenades.SmokeGrenade > 0)
			CurrentActiveExplosiveSlot = EPickupType::SmokeGrenade;
	}
	if (InitSlot != CurrentActiveExplosiveSlot) {
		if (!IsValid(InGameWidget))return;
		InGameWidget->SetActiveExplosive(CurrentActiveExplosiveSlot);
		SERVERRequestChangeActiveExplosive(CurrentActiveExplosiveSlot);
	}
}

void ABRCharacter::OverviewMapClicked(FVector _Location, FKey _Button)
{
	if (EKeys::LeftMouseButton == _Button) {
		if (IsValid(OverviewMarker)) {
			OverviewMarker->SetActorLocation(_Location);
			if (!SoloGame())
				SERVERAddUpdateMarker(_Location);
		}
		else {
			FTransform Trans;
			Trans.SetLocation(_Location);
			OverviewMarker = GetWorld()->SpawnActorDeferred<ABigMapMarker>(OverviewMarkerClass, Trans, nullptr, nullptr,
				ESpawnActorCollisionHandlingMethod::Undefined);
			OverviewMarker->SetLocal(true);
			OverviewMarker->SERVEROwner(this);
			OverviewMarker->SERVERColor(MarkerColor);
			OverviewMarker->FinishSpawning(Trans);
			OverviewMap->GetMinimapComponent()->AddActor(OverviewMarker,false, EMinimapImages::LocationPointer, FVector2D(50.f, 50.f), 
				MarkerColor, false, FText::FromString(TEXT("Marker")));
			MyMinimap->GetMinimapComponent()->AddActor(OverviewMarker, false, EMinimapImages::LocationPointer, FVector2D(50.f, 50.f),
				MarkerColor, false, FText::FromString(TEXT("Marker")));
			if (!SoloGame())
				SERVERAddUpdateMarker(_Location);
		}
	}
	else if (EKeys::RightMouseButton == _Button) {
		if (IsValid(OverviewMarker)) {
			OverviewMarker->Destroy();
			if (!SoloGame())
				SERVERRemoveMarker();
		}
	}
}



void ABRCharacter::ServerRequestChangeActiveExplosive(EPickupType _Type)
{
	int32 Grenades = _Type == EPickupType::FragGrenade ? TotalGrenades.FragGrenade : (_Type == EPickupType::SmokeGrenade ? TotalGrenades.SmokeGrenade : TotalGrenades.StunGrenade);
	if (Grenades <= 0)return;
	if (IsValid(PlayerInfo.Explosive))
		PlayerInfo.Explosive->Destroy();
	FName RowName = _Type == EPickupType::FragGrenade ? TotalGrenades.FragGID : (_Type == EPickupType::SmokeGrenade ? TotalGrenades.SmokeGID : TotalGrenades.StunGID);
	static const FString Context(TEXT("Lookup")); 
	CV(UBRGameInstance, GameInst, GetGameInstance());
	
		if (const FPickupInfos* Row = GameInst->GetItemData(RowName, Context))
		{
			TSubclassOf<AExplosivesItem> ExplosivesClass;
			if (Row->PickupClass && Row->PickupClass->IsChildOf(AExplosivesItem::StaticClass()))
				ExplosivesClass = Row->PickupClass;
			if (!ExplosivesClass)return;
			AExplosivesItem* ExplosiveItem = GetWorld()->SpawnActorDeferred<AExplosivesItem>(ExplosivesClass, FTransform(), nullptr, nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			ExplosiveItem->SetAttachSocketName(Row->EquipSocket);
			ExplosiveItem->SERVERSetNewCount(-1);
			ExplosiveItem->FinishSpawning(FTransform());
			PlayerInfo.Explosive = ExplosiveItem;
			CurrentActiveExplosiveSlot = _Type;
		}

}

bool ABRCharacter::UpdateGrenadeCount()
{
	if (!HasAuthority()) return false;

	if (CurrentActiveExplosiveSlot == EPickupType::FragGrenade) {
		SERVERTotalGrenades(TEXT(""), EPickupType::FragGrenade, TotalGrenades.FragGrenade - 1);
		return TotalGrenades.FragGrenade <= 0;
	}
	else if (CurrentActiveExplosiveSlot == EPickupType::SmokeGrenade) {
		SERVERTotalGrenades(TEXT(""), EPickupType::SmokeGrenade, TotalGrenades.SmokeGrenade - 1);
		return TotalGrenades.SmokeGrenade <= 0;
	
	}
	else if (CurrentActiveExplosiveSlot == EPickupType::StunGrenade) {
		SERVERTotalGrenades(TEXT(""), EPickupType::StunGrenade, TotalGrenades.StunGrenade - 1);
		return TotalGrenades.StunGrenade <= 0;
	}

	return false;
}

FName ABRCharacter::GetCurrentActiveExplosiveSlotName()
{
	if (CurrentActiveExplosiveSlot == EPickupType::FragGrenade) {
		return TotalGrenades.FragGID;
	}
	else if (CurrentActiveExplosiveSlot == EPickupType::SmokeGrenade) {
		return TotalGrenades.SmokeGID;
	}
	else if (CurrentActiveExplosiveSlot == EPickupType::StunGrenade) {
		return TotalGrenades.StunGID;
	}
	return FName();
}


bool ABRCharacter::HasWeapon(ESlotType _Slot)
{
	switch (_Slot)
	{
	
	case ESlotType::Primary:
		if (IsValid(PlayerInfo.PrimaryWeapon) && PlayerInfo.ActiveSlot != ESlotType::Primary)
			return true;

		break;
	case ESlotType::Secondary:
		if (IsValid(PlayerInfo.SecondaryWeapon) && PlayerInfo.ActiveSlot != ESlotType::Secondary)
			return true;
		break;
	case ESlotType::Pistol:
		if (IsValid(PlayerInfo.PistolWeapon) && PlayerInfo.ActiveSlot != ESlotType::Pistol)
			return true;
		break;
	case ESlotType::Melee:
		if (IsValid(PlayerInfo.MeleeWeapon) && PlayerInfo.ActiveSlot != ESlotType::Melee)
			return true;
		break;
	case ESlotType::Explosives:
		if ((TotalGrenades.FragGrenade > 0 || TotalGrenades.SmokeGrenade > 0 || TotalGrenades.StunGrenade > 0) && PlayerInfo.ActiveSlot != ESlotType::Explosives)
		{
			CurrentActiveExplosiveSlot = TotalGrenades.FragGrenade > 0 ? EPickupType::FragGrenade : (TotalGrenades.SmokeGrenade > 0 ? EPickupType::SmokeGrenade : EPickupType::StunGrenade);
			ifv(InGameWidget)
				InGameWidget->SetActiveExplosive(CurrentActiveExplosiveSlot);
			return true;
		}
		break;
	}
	return false;
}
FVector ABRCharacter::GetPickupDropLocation(bool _Rand)
{
	float x = _Rand ? FMath::FRandRange(-20.f, 20.f) : 0.f;
	float y = _Rand ? FMath::FRandRange(-20.f, 20.f) : 0.f;
	FVector Location(x, y,0.f);
	Location += (GetActorLocation() + (GetActorForwardVector() * PickupSpawnDistance));
	return Location;
}
bool ABRCharacter::GetStrafing()
{
	if (UKismetMathLibrary::VSizeXY(GetVelocity()) <= 0)
		return false;
	FVector Velocity(GetVelocity().X, GetVelocity().Y, 0.f);

	FRotator Delta = (Velocity.ToOrientationRotator() - GetBaseAimRotation()).GetNormalized();
	
	float AbsYaw = FMath::Abs(Delta.Yaw);

	return (AbsYaw >= 85.f && AbsYaw <= 95.f);
}

float ABRCharacter::GetFallDamage(float _Veclocity)
{
	if (_Veclocity <= 1000)
		return 10.f;
	else if (_Veclocity <= 2000)
		return 25.f;
	else if (_Veclocity <= 3000)
		return 50.f;
	else
		return 100.f;
}

void ABRCharacter::OnConsumableDroppedOverSlot(int32 _SlotIndex)
{
	if (!IsValid(MyInventory))return;
	UItemWidget* ItemWidget = MyInventory->GetContainer(ESlotType::Inventory)->GetSlotIndexItemWidget(_SlotIndex);
	if (!ItemWidget)return;
	int32 Total;
	const FPickupInfos& Info = MyInventory->GetContainer(ESlotType::Inventory)->FindConsumableTotalInfo(ItemWidget->GetPickupInfo().Type, Total);
	MyInventory->GetContainer(ESlotType::Consumable)->UpdateSlot(Info, 0, ESlotType::Consumable, 0);
	CurrentConsumableSlot = Info.Type;
}

void ABRCharacter::RefreshConsumableSlot(EPickupType _Type)
{
	if (_Type == EPickupType::None || !IsValid(MyInventory))return;
	int32 Total;
	const FPickupInfos& Info  = MyInventory->GetContainer(ESlotType::Inventory)->FindConsumableTotalInfo(_Type, Total);
	if (Total > 0)
		MyInventory->GetContainer(ESlotType::Consumable)->UpdateSlot(Info, 0, ESlotType::Consumable, 0);
	else
		MyInventory->GetContainer(ESlotType::Consumable)->ZeroCount();
}

void ABRCharacter::ServerHandleRequestEnterVehicle(AActor* _Vehicle)
{
	if (!IsValid(_Vehicle))return;
	CV(AVehicle, Vehicle, _Vehicle);
	if (Vehicle && !InVehicle()) {

		if (Vehicle->IsSeatEmpty()) {
			CV(APawn, Pawn, _Vehicle);
			if (!Pawn)return;
			SetInVehicle(true);
			CV(ABRPlayerController, Cont, GetController());
			if (!Cont)return;
			FName SeatName;
			InVehicleSeat= Vehicle->GetEmptySeat(SeatName);
			Cont->CLIENTInVehicleUpdate(true, Pawn, this, InVehicleSeat);
			VehicleRef = Pawn;
			Cont->SetBRPawn(this);
			if (InVehicleSeat != EVehicleSeats::Seat1)
				IsVehicleDriver = false;
			else 
			{
				_Vehicle->SetOwner(GetController());
				GetController()->Possess(Pawn);
				ServerChangeSlot(ESlotType::Inventory);
				IsVehicleDriver = true;
			}
			MCPlayerInVehicle(VehicleRef, SeatName);
			Vehicle->SetSeatOwner(InVehicleSeat, this);
		}
	}
}

void ABRCharacter::ServerHandleExitingVehicle(APawn* _Vehicle)
{
	if (!(IsValid(_Vehicle) && PlayerInfo.Alive && PlayerInfo.Movement == EMovementState::Driving))
		return;
	CV(AVehicle, Vehicle, _Vehicle);
	if (!Vehicle)return;
	FVector Loc = Vehicle->FindEmptySpot(InVehicleSeat);
	Loc += FVector(0.f, 0.f, 50.f);
	this->SetActorLocationAndRotation(Loc, FRotator(0.f, GetActorRotation().Yaw, 0.f));
	if (IsVehicleDriver)
		ControllerRef->Possess(this);
	MCPlayerExitedVehicle();
	Vehicle->SetSeatOwner(InVehicleSeat, nullptr);

}

void ABRCharacter::SetVehicleSeat(EVehicleSeats _Seat, APawn* _VehicleRef)
{
	if (!IsValid(_VehicleRef))return;
	InVehicleSeat = _Seat;
	IsVehicleDriver = _Seat == EVehicleSeats::Seat1 ? true : false;
	VehicleRef = _VehicleRef;
}

bool ABRCharacter::IsGameStarted()
{
	CV(ABRGameState, GameState, UGameplayStatics::GetGameState(GetWorld()));
	if (GameState->GetGameStatus() == EGameStatus::Started)
		return true;
	else
		return false;
}

void ABRCharacter::HandleClothInfoChanged(const FCharacterClothInfo& _ClothInfo, USkeletalMeshComponent* _Mesh, ESlotType _Slot)
{
	_Mesh->SetSkeletalMesh(_ClothInfo.IsEquip ? _ClothInfo.SkeletaAttach : nullptr, false);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,true);
	_Mesh->AttachToComponent(GetMesh(), Rules, _ClothInfo.IsEquip ? _ClothInfo.AttachToSocket : TEXT("None"));
	_Mesh->SetMasterPoseComponent((_ClothInfo.IsEquip && _ClothInfo.ShouldFollowMeshAnimation) ? GetMesh() : nullptr);
	if (!IsLocallyControlled() || !IsValid(PrevCharacter))return;
	PrevCharacter->UpdateClothSlot(_Slot, _ClothInfo);
}


FPickupInfos* ABRCharacter::GetClothDataSlot(ESlotType _Slot)
{
	switch (_Slot)
	{
	case ESlotType::HeadArmor:
		return &HeadArmorInfo.ArmorInfo;
		break;
	case ESlotType::BodyArmor:
		return &BodyArmorInfo.ArmorInfo;
		break;
	case ESlotType::ClothHeadWear:
		return &ClothHeadData;
		break;
	case ESlotType::ClothFaceWear:
		return &ClothFaceData;
		break;
	case ESlotType::ClothEyeWear:
		return &ClothEyeData;
		break;
	case ESlotType::ClothTopWear:
		return &ClothTopData;
		break;
	case ESlotType::ClothPants:
		return &ClothPantsData;
		break;
	case ESlotType::ClothGloves:
		return &ClothGlovesData;
		break;
	case ESlotType::ClothBoots:
		return &ClothBootsData;
		break;
	}
	return nullptr;
}

FCharacterClothInfo ABRCharacter::GetClothInfoSlot(ESlotType _Slot)
{
	switch (_Slot)
	{
	case ESlotType::HeadArmor:
		return ClothArmorHeadInfo;
	case ESlotType::BodyArmor:
		return ClothArmorBodyInfo;
	case ESlotType::ClothHeadWear:
		return ClothHeadWearInfo;
	case ESlotType::ClothFaceWear:
		return ClothFaceWearInfo;
	case ESlotType::ClothEyeWear:
		return ClothEyeWearInfo;
	case ESlotType::ClothTopWear:
		return ClothTopWearInfo;
	case ESlotType::ClothPants:
		return ClothPantsInfo;
	case ESlotType::ClothGloves:
		return ClothGlovesInfo;
	case ESlotType::ClothBoots:
		return ClothBootsInfo;
	}
	return FCharacterClothInfo();
}

void ABRCharacter::OnEquipUpdatePreviewCharacter(ESlotType _Slot)
{
	if (!IsValid(PrevCharacter))return;
	AActor* Weapon = nullptr;;
	switch (_Slot)
	{
	case ESlotType::Primary:
		Weapon = PlayerInfo.PrimaryWeapon;
		break;
	case ESlotType::Secondary:
		Weapon = PlayerInfo.SecondaryWeapon;
		break;
	case ESlotType::Pistol:
		Weapon = PlayerInfo.PistolWeapon;
		break;
	case ESlotType::Melee:
		Weapon = PlayerInfo.MeleeWeapon;
		break;
	}

	PrevCharacter->WeaponEquip(Weapon, _Slot);
}

void ABRCharacter::ClearClothSlot(ESlotType _Slot)
{
	switch (_Slot)
	{
	case ESlotType::HeadArmor:
		SERVERClothInfo(ESlotType::HeadArmor,FCharacterClothInfo());
		break;
	case ESlotType::BodyArmor:
		SERVERClothInfo(ESlotType::BodyArmor, FCharacterClothInfo());
		break;
	case ESlotType::ClothHeadWear:
		SERVERClothInfo(ESlotType::ClothHeadWear, FCharacterClothInfo());
		break;
	case ESlotType::ClothFaceWear:
		SERVERClothInfo(ESlotType::ClothFaceWear, FCharacterClothInfo());
		break;
	case ESlotType::ClothEyeWear:
		SERVERClothInfo(ESlotType::ClothEyeWear, FCharacterClothInfo());
		break;
	case ESlotType::ClothTopWear:
		SERVERClothInfo(ESlotType::ClothTopWear, FCharacterClothInfo());
		break;
	case ESlotType::ClothPants:
		SERVERClothInfo(ESlotType::ClothPants, FCharacterClothInfo());
		break;
	case ESlotType::ClothGloves:
		SERVERClothInfo(ESlotType::ClothGloves, FCharacterClothInfo());
		break;
	case ESlotType::ClothBoots:
		SERVERClothInfo(ESlotType::ClothBoots, FCharacterClothInfo());
		break;
	}
}

bool ABRCharacter::CanAddedInventory(int32 _Count, float _Weight, float _MinusWeight,float& _NewWeigh)
{
	_NewWeigh = CurrentInventoryCapacity + (_Count * _Weight) - _MinusWeight;
	if (_NewWeigh > MaxInventoryCapacity)
		return false;
	else
		return true;
}
void ABRCharacter::OnPickupEquipChanged_Implementation
(bool _Equip, AActor* _Pickup, FName _ToSocket, bool _Attach, bool _SetInvisile)
{
	if (!IsValid(_Pickup))return;
	if(_Equip)
		EquipSocket(_Pickup, _ToSocket, _Attach);
	else {
		CV(APickupItem, Item, _Pickup);
		Item->PickupNotEquip();
	}
	if(_SetInvisile)
	_Pickup->GetRootComponent()->SetVisibility(false, true);
}
void ABRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(ABRCharacter, ClothHeadWearInfo);
	DOREPLIFETIME(ABRCharacter, ClothFaceWearInfo);
	DOREPLIFETIME(ABRCharacter, ClothEyeWearInfo);
	DOREPLIFETIME(ABRCharacter, ClothTopWearInfo);
	DOREPLIFETIME(ABRCharacter, ClothPantsInfo);
	DOREPLIFETIME(ABRCharacter, ClothGlovesInfo);
	DOREPLIFETIME(ABRCharacter, ClothBootsInfo);

	DOREPLIFETIME(ABRCharacter, ClothArmorHeadInfo);
	DOREPLIFETIME(ABRCharacter, ClothArmorBodyInfo);

	DOREPLIFETIME_CONDITION(ABRCharacter, TotalAmmo, COND_OwnerOnly);
	//COND_OwnerOnly 의미: 이 속성/변수는 배우(Actor)의 “소유자(Owner)”에게만 네트워크로 전송됨.
	//결과: 서버→오너 클라이언트 만 값이 바뀌고, 다른 클라이언트들은 전혀 받지 않음. (서버는 항상 최신값을 가짐)
	DOREPLIFETIME(ABRCharacter, TotalGrenades);
	DOREPLIFETIME_CONDITION(ABRCharacter, Aiming, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, RepHealth, COND_SkipOwner);
	//COND_SkipOwner 는 해당 변수의 복제를 “오너(Owner)에게만 제외” 하고 나머지 모든 클라이언트에게 전송하는 조건입니다.
	//(서버는 항상 최신값을 갖고, 오너 클라이언트는 이 값의 네트워크 업데이트를 받지 않습니다.)
	DOREPLIFETIME(ABRCharacter, IsPlayerDown);
	DOREPLIFETIME(ABRCharacter, BeingRevived);
	DOREPLIFETIME(ABRCharacter, MarkerColor);
	DOREPLIFETIME(ABRCharacter, TeamID);
	DOREPLIFETIME_CONDITION(ABRCharacter, WalkingState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, BtnFwdDown, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, BtnRightDown, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, BtnLeftDown, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, GoingBackWards, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, RepLeaning, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, RepControl, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, IsFiring, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ABRCharacter, RepActiveSlot, COND_SkipOwner);
	DOREPLIFETIME(ABRCharacter, KillCount);
	DOREPLIFETIME_CONDITION(ABRCharacter, IsVehicleDriver, COND_SkipOwner);
	DOREPLIFETIME(ABRCharacter, JumpingOffPlane);
	DOREPLIFETIME(ABRCharacter, Parachuting);
	DOREPLIFETIME(ABRCharacter, RepInVehicle);
		
		
}

//dd
void ABRCharacter::GameStarted(bool _InPlane)
{
	
	CurrentConsumableSlot = EPickupType::None;
	CurrentInventoryCapacity = 0.f;
	
	if(HasAuthority()) {
		TArray<AActor*> Weapons;
		Weapons.Add(PlayerInfo.PrimaryWeapon);
		Weapons.Add(PlayerInfo.SecondaryWeapon);
		Weapons.Add(PlayerInfo.PistolWeapon);
		Weapons.Add(PlayerInfo.MeleeWeapon);
		Weapons.Add(PlayerInfo.Explosive);
		Weapons.Add(PlayerInfo.BackpackRef);
		for (int32 i = 0; i < Weapons.Num(); i++) {
			if (IsValid(Weapons[i]))
				Weapons[i]->Destroy();
		}
		RepActiveSlot = ESlotType::Inventory;
		ServerInventoryItems.Empty();
		ClearAllCloth();
		GetWorldTimerManager().SetTimer(HandleWaterDepletionAndBoostHandle,this,&ABRCharacter::HandleWaterDepletionAndBoost,1.0f,true);
		if (IsLocallyControlled()) {
			if (IsValid(PrevCharacter))
				PrevCharacter->ClearAllSlots();
		}
		else
			CLIENTGameStarted(_InPlane);
	}
	if (PlayerInfo.Movement == EMovementState::Driving)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SetInVehicle(false);
		VehicleRef = nullptr;
		IsVehicleDriver = false;
	}
	PlayerInfo.Health = 100.f;
	PlayerInfo.ActiveSlot = ESlotType::Inventory;
	PlayerInfo.Movement = _InPlane ? EMovementState::InPlane : EMovementState::Moving;
	PlayerInfo.BackpackLV = 0;
	PlayerInfo.WaterPercent = 100.f;
	PlayerInfo.BoostPercent = 0.f;
	if (!IsLocallyControlled()) return;
	ActiveSlotChanged(ESlotType::Inventory);
	MyInventory->RemoveFromParent();
	MyInventory = Cast<UInventoryWidget>(CreateWidget(GetWorld(), MyInventoryClass));
	MyInventory->SetOwningPlayer(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MyInventory->AddToViewport();
	MyInventory->SetVisibility(ESlateVisibility::Hidden);
	MyInventory->UpdateInventoryWeight(CurrentInventoryCapacity, MaxInventoryCapacity);
	UWidgetBlueprintLibrary::CancelDragDrop();
	BindInventoryevents();
	RepActiveSlot = ESlotType::Inventory;
	EquipInfoW->RemoveFromParent();
	EquipInfoW = Cast<UEquipInfoWidget>(CreateWidget(GetWorld(), EquipInfoWClass));
	EquipInfoW->AddToViewport();
	CV(ABRPlayerController, Cont, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FInputModeGameOnly InputMode;
	Cont->SetInputMode(InputMode);
	Cont->bShowMouseCursor = false;
}

bool ABRCharacter::SoloGame()
{
	CV(ABRGameState, GameState, UGameplayStatics::GetGameState(GetWorld()));
	if(!GameState) return false;
	
	return GameState->GetBRGameMode() == EGameModeList::Solo;
}



void ABRCharacter::BindInventoryevents()
{
	if (!IsLocallyControlled())return;

	for (int32 i = 0; i <= static_cast<int32>(ESlotType::ClothEnd); i++) {
		ESlotType Type = static_cast<ESlotType>(i);
		UItemsContainer* Container = MyInventory->GetContainer(Type);
		if (!IsValid(Container) || Type == ESlotType::Consumable) {
			Container = nullptr;
			continue;
		}
		Container->SetDragDropContainerDelegate(this,&ABRCharacter::ClientInventoryDragDrop);
		Container->SetPlayerOwner(this);
	}
	MyInventory->GetContainer(ESlotType::Consumable)->SetDragDropContainerDelegate(this, &ABRCharacter::EventOnDropConsumables);
	MyInventory->GetInventoryBackground()->SetPlayerOwner(this);
}


void ABRCharacter::SetMinimapOwner(APawn* _Panw)
{
	MyMinimap->GetMinimapComponent()->MinimapStartUpdate(_Panw);
	OverviewMap->GetMinimapComponent()->MinimapStartUpdate(_Panw);
}

void ABRCharacter::AddPlaneMinimap(AActor* _PlaneActor)
{
	MyMinimap->GetMinimapComponent()->ToggleMyLocationVisibility();
	OverviewMap->GetMinimapComponent()->ToggleMyLocationVisibility();
	MyMinimap->GetMinimapComponent()->
		AddActor(_PlaneActor, true, EMinimapImages::PlaneT, FVector2D(50.f, 50.f), FLinearColor(1.0f, 0.620274f, 0.f), false, FText::FromString(TEXT("Plane")));
	OverviewMap->GetMinimapComponent()->
		AddActor(_PlaneActor, true, EMinimapImages::PlaneT, FVector2D(50.f, 50.f), FLinearColor(1.0f, 0.620274f, 0.f), false, FText::FromString(TEXT("Plane")));
}

void ABRCharacter::UpdateArmor(ESlotType _Slot, float _NewArmor)
{
	if (!(_Slot == ESlotType::HeadArmor || _Slot == ESlotType::BodyArmor))
		return;
	if (_Slot == ESlotType::HeadArmor) {
		HeadArmorInfo.ArmorValue = _NewArmor;
	}
	else if (_Slot == ESlotType::BodyArmor) {
		BodyArmorInfo.ArmorValue = _NewArmor;
	}
	CLIENTUpdateArmorValue(_Slot, _NewArmor);

}

void ABRCharacter::OnSpectated()
{
	if (!IsLocallyControlled())return;

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bUsePawnControlRotation = false;
	SetCameraSpect = true;
	TeammateIconWidget->SetVisibility(false, false);

}

void ABRCharacter::HandlePlayerDeath()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this] {
		Destroy();
		}, 5.0f, false);
}




void ABRCharacter::SetupTeammateIcon(AActor* _Player)
{
	LocalPlayerRef = _Player;
	CV(UTeammateIcon, Icon, TeammateIconWidget->GetUserWidgetObject());
	if (!Icon)return;
	Icon->Initialize(MarkerColor.GetSpecifiedColor());
	TeammateIconWidget->SetHiddenInGame(false, false);
}

void ABRCharacter::ClientYouWin_Implementation()
{
	MyInventory->RemoveFromParent();
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsFiring)return;
	IsFiring = false;
	SERVERStopFiring();
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	if (IsValid(Weapon)) {
		CV(AWeaponItem, Item, Weapon);
		if(IsValid(Item))
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle,  [this,Item] {
				Item->StopSoundandEffect();
				}, 1.0f, false);
		}
	}
}

void ABRCharacter::MCMulticastDoPlayerExitedVehicle_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Multicast 실행 중! (모든 클라이언트에서 동작)"));
}

void ABRCharacter::SERVERRequestDoPlayerExitedVehicle_Implementation()
{
	MCMulticastDoPlayerExitedVehicle();
}


void ABRCharacter::SERVERRequestEnterVehicle_Implementation(AActor* _Vehicle)
{
	ServerHandleRequestEnterVehicle(_Vehicle);
}

void ABRCharacter::SERVERRequestExitVehicle_Implementation()
{
	ServerHandleExitingVehicle(VehicleRef);
}

void ABRCharacter::SERVERPickupBackPack_Implementation(ABackpackItem* _Pickup)
{
	ServerPickupBackpack(_Pickup);
}


void ABRCharacter::SERVERPickupItem_Implementation(AActor* _Pickup)
{
	if (IsValid(_Pickup)) 
		ServerAttemptPickupItem(_Pickup);
}


void ABRCharacter::SERVERRequestLootBoxInteract_Implementation(AActor* _LootBoxActor)
{
	CV(ALootBox, LootBox, _LootBoxActor);
	CV(AAirDropBox,AirDropBox, _LootBoxActor)
	if (IsValid(LootBox))
		LootBox->OnBeginInteract(this);
	else if (IsValid(AirDropBox))
		AirDropBox->OnBeginInteract(this);
}



void ABRCharacter::SERVERPrimarySecondarySwap_Implementation()
{
	ServerSwapPrimarySecondary();
}

void ABRCharacter::SERVEREquipInventoryMainSlot_Implementation(ESlotType _Slot, int32 _UniqueID, int32 _SlotIndex)
{
	ServerEquipInventoryMainSlot(_Slot, _UniqueID, _SlotIndex);
}

void ABRCharacter::SERVERDragDropEquipInventory_Implementation(ESlotType _SlotType, int32 _UniqueID, int32 _SlotIndex)
{
	ServerEquipInventory(_SlotType, _UniqueID, _SlotIndex);
}


void ABRCharacter::SERVERSetGoingBackWards_Implementation(bool _GoingBackWards)
{
	GoingBackWards = _GoingBackWards;
}

void ABRCharacter::SERVERDropInventoryItem_Implementation(int32 _UniqueID, int32 _Count)
{
	ServerDropInventoryItem(_UniqueID, _Count);	
}

void ABRCharacter::SERVERDropEquipItem_Implementation(ESlotType _Slot)
{
	ServerDropEquipItem(_Slot);
}


void ABRCharacter::CLIENTUpdateInventoryItemCount_Implementation(int32 _UniqueServerID, int32 _NewCount, float _NewCapacity)
{
	CurrentInventoryCapacity = _NewCapacity;
	MyInventory->GetContainer(ESlotType::Inventory)->UpdateCountID(_UniqueServerID, _NewCount);
	MyInventory->UpdateInventoryWeight(CurrentInventoryCapacity, MaxInventoryCapacity);
	RefreshConsumableSlot(CurrentConsumableSlot);
}

void ABRCharacter::CLIENTAddNewInventoryItem_Implementation(FName _PickupID, int32 _UniqueServerID, AActor* _Pickup, int32 _NewCapacity, int32 _SlotIndex, bool _ClearSlot, int32 _Count)
{
	if (_SlotIndex != -1 && _ClearSlot) 
		GetContainerSlot(ESlotType::Inventory)->ClearSlot(_SlotIndex);
	CurrentInventoryCapacity = _NewCapacity;
	if (IsValid(_Pickup))
	{
		CV(APickupItem, Item, _Pickup);
		auto Info = Item->GetInfo();
		MyInventory->GetContainer(ESlotType::Inventory)->AddNewItem(Info, ESlotType::Inventory, _SlotIndex, _UniqueServerID);
	}
	else {
		CV(UBRGameInstance, GameInst, GetGameInstance());

			if (const FPickupInfos* Row = GameInst->GetItemData(_PickupID))
			{
				MyInventory->GetContainer(ESlotType::Inventory)->AddNewItem(*Row, ESlotType::Inventory, _SlotIndex, _UniqueServerID);
			}
		
	}
	MyInventory->UpdateInventoryWeight(CurrentInventoryCapacity, MaxInventoryCapacity);
	RefreshConsumableSlot(CurrentConsumableSlot);

}

void ABRCharacter::CLIENTPickupEquip_Implementation(FName _PickupID, ESlotType _Slot, AActor* _Pickup, float _newWeight, int32 _SlotIndex, bool _Replaced)
{
	bool Find = false;
	FPickupInfos Info;
	if (!IsValid(_Pickup)) {
		CV(UBRGameInstance, GameInst, GetGameInstance());
			if (const FPickupInfos* Row = GameInst->GetItemData(_PickupID)) {
				Info = *Row;
				Find = true;
			}
	}
	else
	{
		CV(APickupItem, Item, _Pickup);
		Info = Item->GetInfo();
		Find = true;
	}
	if (Find) {
		switch (_Slot)
		{
		case ESlotType::Primary:
		case ESlotType::Secondary:
		case ESlotType::Pistol:
		case ESlotType::Melee:
			SetWeaponRefSlot(_Slot,_Pickup);
			MyInventory->PickupEquip(_Slot, Info);
			EquipInfoW->PickupEquip(_Slot, Info);
			OnEquipUpdatePreviewCharacter(_Slot);
			break;
		case ESlotType::Explosives:
			SetWeaponRefSlot(_Slot, _Pickup);
			MyInventory->PickupEquip(_Slot, Info);
			EquipInfoW->PickupEquip(_Slot, Info);
			break;
		case ESlotType::HeadArmor:
			HeadArmorInfo.HasArmor = true;
			MyInventory->PickupEquip(_Slot, Info);
			EquipInfoW->PickupEquip(_Slot, Info);
			break;
		case ESlotType::BodyArmor:
			BodyArmorInfo.HasArmor = true;
			MyInventory->PickupEquip(_Slot, Info);
			EquipInfoW->PickupEquip(_Slot, Info);
			break;
		case ESlotType::ClothHeadWear:
		case ESlotType::ClothFaceWear:
		case ESlotType::ClothEyeWear:
		case ESlotType::ClothTopWear:
		case ESlotType::ClothPants:
		case ESlotType::ClothGloves:
		case ESlotType::ClothBoots:
			MyInventory->PickupEquip(_Slot, Info);
			break;
		}
	}
	CurrentInventoryCapacity = _newWeight;
	MyInventory->UpdateInventoryWeight(CurrentInventoryCapacity, MaxInventoryCapacity);
	if (_SlotIndex != -1)
		GetContainerSlot(ESlotType::Inventory)->ClearSlot(_SlotIndex);
	if (_Replaced)
		ActiveSlotChanged(RepActiveSlot);
}


void ABRCharacter::CLIENTUpdateArmorValue_Implementation(ESlotType _Slot, float _NewValue)
{
	
	MyInventory->UpdateArmorValue(_Slot, _NewValue);
	if (ESlotType::HeadArmor == _Slot) {
		HeadArmorInfo.HasArmor = true;
		HeadArmorInfo.ArmorValue = _NewValue;
	}
	else {
		BodyArmorInfo.HasArmor = true;
		BodyArmorInfo.ArmorValue = _NewValue;
	}
}

void ABRCharacter::MCOnItemDrop_Implementation(AActor* _Pickup, FVector _Location)
{
	DropPickupGround(_Pickup, _Location);
}


void ABRCharacter::CLIENTEquipWeaponDrop_Implementation(ESlotType _Slot)
{
	UItemsContainer* Container = GetContainerSlot(_Slot);
	UItemsContainer* InfoContainer = GetInfoContainerSlot(_Slot);
	SetWeaponRefSlot(_Slot, nullptr);
	Container->ClearSlot(0);
	ifv(InfoContainer) {
		InfoContainer->ClearSlot(0);
	}
	OnEquipUpdatePreviewCharacter(_Slot);
}


void ABRCharacter::CLIENTPrimarySecondarySwapped_Implementation(AActor* _Primary, AActor* _Secondary, ESlotType _Slot)
{
	if (!IsValid(_Primary))
		GetContainerSlot(ESlotType::Primary)->ClearSlot(0);
	else
	{
		PlayerInfo.PrimaryWeapon = _Primary;
		CV(APickupItem, Item, PlayerInfo.PrimaryWeapon);
		auto& Info = Item->GetInfo();
		MyInventory->PickupEquip(ESlotType::Primary, Info);
		EquipInfoW->PickupEquip(ESlotType::Primary, Info);
	}
	if (!IsValid(_Secondary))
		GetContainerSlot(ESlotType::Secondary)->ClearSlot(0);
	else
	{
		PlayerInfo.SecondaryWeapon = _Secondary;
		CV(APickupItem, Item, PlayerInfo.SecondaryWeapon);
		auto& Info = Item->GetInfo();
		MyInventory->PickupEquip(ESlotType::Secondary, Info);
		EquipInfoW->PickupEquip(ESlotType::Secondary, Info);
	}
	ifv(PrevCharacter)
		PrevCharacter->PrimaySecondarSwitch();
	if (PlayerInfo.ActiveSlot != _Slot)
		ActiveSlotChanged(_Slot);
}

void ABRCharacter::CLIENTUpdateHealth_Implementation(float _NewHealth, float _Water, float _Boost)
{
	if (IsValid(InGameWidget)) {
		InGameWidget->UpdateHealth(_NewHealth, IsPlayerDown);
		InGameWidget->UpdateWater(_Water);
		InGameWidget->UpdateBoost(_Boost);
		PlayerInfo.Health = _NewHealth;
		PlayerInfo.WaterPercent = _Water;
		PlayerInfo.BoostPercent = _Boost;
	}
	else {
		ControllerRef->CLIENTUpdateHealth(_NewHealth, _Water, _Boost);
	}
}


void ABRCharacter::MCClientYouDied_Implementation()
{
	IsFiring = false;
	ServerStopFiring();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true, true);
	if (IsLocallyControlled()) {
		MyInventory->RemoveFromParent();
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UWidgetBlueprintLibrary::CancelDragDrop();
		if (IsValid(RevivingPlayer)) {
			InGameWidget->StopRevive();
			RevivingPlayer = nullptr;
		}
	}
	if (PlayerInfo.Movement == EMovementState::Driving && HasAuthority()){
		CV(AVehicle, Vehicle, VehicleRef);
		Vehicle->SetSeatOwner(InVehicleSeat, nullptr);
	}
}

void ABRCharacter::CLIENTShowHitmarker_Implementation()
{
	ifv(InGameWidget) {
		InGameWidget->ShowHitMarker();
	}
	else {
		ControllerRef->CLIENTShowHitmarker();
	}
}


void ABRCharacter::MCPlaySound_Implementation(USoundBase* _Sound, FVector _Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), _Sound, _Location);
}


void ABRCharacter::CLIENTBackPackEquip_Implementation(float _MaxInvCapacity, int32 _BackpackLV, const FPickupInfos& _BackpackInfo)
{
	MaxInventoryCapacity = _MaxInvCapacity;
	MyInventory->UpdateInventoryWeight(CurrentInventoryCapacity, MaxInventoryCapacity);
	PlayerInfo.BackpackLV = _BackpackLV;
	MyInventory->UpdateBackPackLevel(_BackpackLV);
	MyInventory->PickupEquip(ESlotType::Backpack, _BackpackInfo);
	EquipInfoW->PickupEquip(ESlotType::Backpack, _BackpackInfo);
	if (IsValid(PrevCharacter))
		PrevCharacter->BackpackEquip(_BackpackInfo);
}


void ABRCharacter::SERVERRequestOpenParachute_Implementation()
{
	if (JumpingOffPlane && !Parachuting)
		MCOpenParachute();
}


void ABRCharacter::SERVEROnReviveDone_Implementation(ABRCharacter* _RevivingPlayer)
{
	if (IsValid(_RevivingPlayer))
		_RevivingPlayer->ServerOnReviveDone();
}


void ABRCharacter::CLIENTGameStarted_Implementation(bool _InPlane)
{
	GameStarted(_InPlane);
	UsingItem = false;
	if (IsValid(PrevCharacter))
		PrevCharacter->ClearAllSlots();
}



void ABRCharacter::MCPlayerInVehicle_Implementation(AActor* _Vehicle, FName _SeatName)
{
	SetInVehicle(true);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	USkeletalMeshComponent* TargetMesh = _Vehicle->FindComponentByClass<USkeletalMeshComponent>();
	if (TargetMesh)
	{
		AttachToComponent(TargetMesh, Rules, _SeatName);
	}
	//AttachToActor(_Vehicle, Rules, _SeatName);
	
}


void ABRCharacter::SERVERRequestChangeActiveExplosive_Implementation(EPickupType _Type) 
{
	ServerRequestChangeActiveExplosive(_Type);
}



void ABRCharacter::SERVERPlayMontageToAll_Implementation(UAnimMontage* _Montage)
{
	MCPlayMontage(_Montage);
}

void ABRCharacter::SERVERSetBtnFwdDown_Implementation(bool _BtnFwdDown)
{
	BtnFwdDown = _BtnFwdDown;
}
void ABRCharacter::MCPlayMontage_Implementation( UAnimMontage* _Montage)
{
	if (IsLocallyControlled())return;
	AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
	AnimInstance->Montage_Play(_Montage);
}
void ABRCharacter::SERVERUpdateCameraMode_Implementation(bool _IsThirdPerson)
{
	PlayerInfo.isThirdPerson = _IsThirdPerson;
}


void ABRCharacter::SERVERFireWeapon_Implementation()
{
	ServerFireWeapon();
}

void ABRCharacter::ServerFireWeapon()
{
	if (PlayerInfo.ActiveSlot == ESlotType::Primary || PlayerInfo.ActiveSlot == ESlotType::Secondary || PlayerInfo.ActiveSlot == ESlotType::Pistol) {
		if (IsValid(GetCurrentMontage()))
			MCStopMontage();
		AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
		CV(AWeaponItem, Item, Weapon);
		if (!Item)return;

		if (!Item->CanFire())return;
		IsFiring = true;
		Item->StartFire();

	}
}

void ABRCharacter::SERVERStopFiring_Implementation()
{
	ServerStopFiring();
}

void ABRCharacter::ServerStopFiring()
{
	IsFiring = false;
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	CV(AWeaponItem, Item, Weapon);
	if (!Item)return;
	Item->StopFire();
	CLIENTFireStopUpdateAmmo(Item->GetCurrentAmmo());
}


void ABRCharacter::SERVERThrowGrenade_Implementation()
{
	ServerThrowGrenade();
}

void ABRCharacter::ServerThrowGrenade()
{
	if ((TotalGrenades.FragGrenade > 0 || TotalGrenades.SmokeGrenade > 0 || TotalGrenades.StunGrenade > 0) && PlayerInfo.ActiveSlot == ESlotType::Explosives) {
		AActor* Weapon = GetWeaponSlot(ESlotType::Explosives);
		CV(AExplosivesItem, Explosive, Weapon);
		if (!IsValid(Explosive))return;
		Explosive->SpawnExplosive(NadeLoc->GetComponentLocation(), FRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0.f), this);
		ServerInventoryExplosiveUsed(GetCurrentActiveExplosiveSlotName());
		if (!UpdateGrenadeCount())return;
		MCActiveSlotChanged(ESlotType::Inventory);
		Weapon->Destroy();
	}
}

void ABRCharacter::SERVERMeleeWeaponAtkConnect_Implementation(ACharacter* _Char, FVector _Location)
{
	ServerMeleeWeaponAtkConnect(_Char, _Location);
}

void ABRCharacter::ServerMeleeWeaponAtkConnect(ACharacter* _Char, FVector _Location)
{
	AActor* Weapon = GetWeaponSlot(ESlotType::Melee);
	CV(AMeleeWeaponItem, MeleeWeapon, Weapon);
	if (!MeleeWeapon)return;
	if (IsValid(_Char)) {
		CV(ABRCharacter, Char, _Char);
		if (Char) {
			auto& Info = MeleeWeapon->GetInfo();
			Char->DamageTaken(MeleeWeapon->GetDamage(), Info.Name.ToString(), this, false);
		}
		else
			UGameplayStatics::ApplyDamage(_Char, MeleeWeapon->GetDamage(), nullptr, this, UMeleeDamage::StaticClass());
	}
	if (IsValid(MeleeWeapon->GetMeleeHitSound())) {
		MCPlaySound(MeleeWeapon->GetMeleeHitSound(), _Location);
	}
}

void ABRCharacter::SERVERReloadWeapon_Implementation()
{
	ServerReloadWeapon();
}
void ABRCharacter::ServerReloadWeapon()
{
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	if (!IsValid(Weapon))return;
	CV(AWeaponItem, GunInter, Weapon);
	if (!GunInter)return;

	if (!GunInter->GetCanReload() || GetTotalAmmoCountAmmoType(GunInter->GetAmmoType()) <= 0)return;
	auto AmmoPickupType = GetPickupTypeAmmoType(GunInter->GetAmmoType());
	bool RemoveItem = false;
	for (int32 i = 0; i < ServerInventoryItems.Num(); i++) {
		if (ServerInventoryItems[i].Type != AmmoPickupType)continue;
		int32 Used; int32 RemainingAmmo;
		GunInter->Reload(ServerInventoryItems[i].Count, Used, RemainingAmmo);
		if (RemainingAmmo > 0)
			ServerInventoryItems[i].Count = RemainingAmmo;
		else
			RemoveItem = true;
		SERVERTotalAmmo(AmmoPickupType, RemainingAmmo);
		CurrentInventoryCapacity -= Used * ServerInventoryItems[i].Weight;
		CLIENTUpdateInventoryItemCount(ServerInventoryItems[i].ServerUniqueID, RemoveItem ? 0 : ServerInventoryItems[i].Count, CurrentInventoryCapacity);
		if (RemoveItem)
			ServerInventoryItems.RemoveAt(i);
		break;
	}

}

void ABRCharacter::SERVERChangeSlot_Implementation(ESlotType _Slot)
{
	ServerChangeSlot(_Slot);
}

void ABRCharacter::SERVERSetAiming_Implementation(bool _Aiming)
{
	Aiming = _Aiming;
}

void ABRCharacter::SERVERSetWalkingState_Implementation(EWalkingState _State)
{
	SetWalkingState(_State);
}

void ABRCharacter::SERVERUseItem_Implementation(int32 _ServerID)
{
	ServerUseConsumable(_ServerID);
}
void ABRCharacter::MCStopMontage_Implementation()
{
	if(!IsLocallyControlled())
		AnimInstance->Montage_Stop(0.f, GetCurrentMontage());
}


void ABRCharacter::CLIENTFireStopUpdateAmmo_Implementation(int32 _Ammo)
{
	ClientFireStopUpdateAmmo(_Ammo);
}

void ABRCharacter::ClientFireStopUpdateAmmo(int32 _Ammo)
{
	AActor* Weapon = GetWeaponSlot(PlayerInfo.ActiveSlot);
	CV(AWeaponItem, Item, Weapon);
	if(!IsValid(Item))return;
	Item->SetCurrentMagSize(_Ammo);
	if (!IsValid(InGameWidget))return;
	InGameWidget->UpdateAmmoCount(GetWeaponAmmoCurrentMagSize(), GetWeaponAmmoTotal());

}
void ABRCharacter::CLIENTPlayerInZoneStatus_Implementation(bool _InZone)
{
	PlayerInZoneStatus(_InZone);
}

void ABRCharacter::PlayerInZoneStatus(bool _InZone)
{
	if (IsLocallyControlled())
		PostProcessOutOfZone->bEnabled = !_InZone;
}
void ABRCharacter::SetMarkerColor(FSlateColor _MarkerColor)
{
	if (MarkerColor != _MarkerColor)
	{
		MarkerColor = _MarkerColor;

	}
}
void ABRCharacter::MCActiveSlotChanged_Implementation(ESlotType _Slot)
{
	ActiveSlotChanged(_Slot);
	RepActiveSlot = _Slot;
	if (ESlotType::Explosives != _Slot) {
		CurrentActiveExplosiveSlot = EPickupType::None;
		if (IsLocallyControlled())
			InGameWidget->SetActiveExplosive(EPickupType::None);
	}
}


void ABRCharacter::MCOpenParachute_Implementation()
{
	mcOpenParachute();
	
}
void ABRCharacter::MCPlayerExitedVehicle_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	SetInVehicle(false);
	VehicleRef = nullptr;
	IsVehicleDriver = false;
}
void ABRCharacter::mcOpenParachute()
{
	if (HasAuthority()) {
		Parachuting = true;
		HandleParachutingChanged();
	}
	Parachute = GetWorld()->SpawnActor<AActor>(ParachuteClass, FTransform());
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Parachute->AttachToComponent(GetMesh(), Rules, TEXT("Parachute"));
	CboomDefaultRelative = CameraBoom->GetRelativeLocation();
	CameraBoom->SetRelativeLocation(CboomDefaultRelative + FVector(-750.f, 0.f, 750.f));
	GetCharacterMovement()->GravityScale = ParachuteGravityScale;

}


void ABRCharacter::SERVERPunchConnect_Implementation(ACharacter* _Char, FVector _Location)
{
	ServerPunchConnect(_Char, _Location);
}

void ABRCharacter::SERVERSetBtnRightDown_Implementation(bool _BtnRight, bool _BtnLeft)
{
	BtnRightDown = _BtnRight;
	BtnLeftDown = _BtnLeft;

}

void ABRCharacter::SERVERSetLeaning_Implementation(float _Leaning)
{
	RepLeaning = _Leaning;
}


void ABRCharacter::SERVERAddUpdateMarker_Implementation(FVector _Location)
{
	if (IsValid(OverviewMarker))
		OverviewMarker->SetActorLocation(_Location);
	else {
		FTransform Trans;
		Trans.SetLocation(_Location);
		OverviewMarker = GetWorld()->SpawnActorDeferred<ABigMapMarker>(OverviewMarkerClass, Trans, nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::Undefined);
		OverviewMarker->SetLocal(false);
		OverviewMarker->SERVEROwner(this);
		OverviewMarker->SERVERColor(MarkerColor);
		OverviewMarker->FinishSpawning(Trans);
	}
}

void ABRCharacter::SERVERRemoveMarker_Implementation()
{
	if (IsValid(OverviewMarker))
		OverviewMarker->Destroy();
}

void ABRCharacter::ServerPunchConnect(ACharacter* _Char, FVector _Location)
{
	MCPlaySound(PunchSound, _Location);
	if (!IsValid(_Char))return;
	CV(ABRCharacter, Char, _Char);
	if (Char)
		Char->DamageTaken(PunchDamage, TEXT("Punch"), this, false);
	else {
		UGameplayStatics::ApplyDamage(_Char, PunchDamage, nullptr, this, UMeleeDamage::StaticClass());
	}
}
//OnRep

void ABRCharacter::OnRep_ClothHeadWearInfo()
{
	HandleClothInfoChanged(ClothHeadWearInfo, HeadWearSkelMesh,ESlotType::ClothHeadWear);
}

void ABRCharacter::OnRep_ClothFaceWearInfo()
{
	HandleClothInfoChanged(ClothFaceWearInfo, FaceWearSkelMesh, ESlotType::ClothFaceWear);

}
void ABRCharacter::OnRep_ClothEyeWearInfo()
{
	HandleClothInfoChanged(ClothEyeWearInfo, EyeWearSkelMesh, ESlotType::ClothEyeWear);
}

void ABRCharacter::OnRep_ClothTopWearInfo()
{
	HandleClothInfoChanged(ClothTopWearInfo, TopWearSkelMesh, ESlotType::ClothTopWear);
}

void ABRCharacter::OnRep_ClothPantsInfo()
{
	HandleClothInfoChanged(ClothPantsInfo, PantsSkelMesh, ESlotType::ClothPants);
}

void ABRCharacter::OnRep_ClothGlovesInfo()
{
	HandleClothInfoChanged(ClothGlovesInfo, GlovesSkelMesh, ESlotType::ClothGloves);
}

void ABRCharacter::OnRep_ClothBootsInfo()
{
	HandleClothInfoChanged(ClothBootsInfo, BootsSkelMesh, ESlotType::ClothBoots);
}

void ABRCharacter::OnRep_ClothArmorHeadInfo()
{
	HandleClothInfoChanged(ClothArmorHeadInfo, HeadArmorSkelMesh, ESlotType::HeadArmor);
}

void ABRCharacter::OnRep_ClothArmorBodyInfo()
{
	
	HandleClothInfoChanged(ClothArmorBodyInfo, ArmorSkelMesh, ESlotType::BodyArmor);
}

void ABRCharacter::SERVERClothInfo_Implementation(ESlotType _Type, FCharacterClothInfo _Info)
{
	if (!HasAuthority()) return;

	switch (_Type)
	{
	case ESlotType::HeadArmor:
		ClothArmorHeadInfo = _Info;
		HandleClothInfoChanged(ClothArmorHeadInfo, HeadArmorSkelMesh, ESlotType::HeadArmor);
		break;
	case ESlotType::BodyArmor:
		ClothArmorBodyInfo = _Info;
		HandleClothInfoChanged(ClothArmorBodyInfo, ArmorSkelMesh, ESlotType::BodyArmor);
		break;
	case ESlotType::ClothHeadWear:
		ClothHeadWearInfo = _Info;
		HandleClothInfoChanged(ClothHeadWearInfo, HeadWearSkelMesh, ESlotType::ClothHeadWear);
		break;
	case ESlotType::ClothFaceWear:
		ClothFaceWearInfo = _Info;
		HandleClothInfoChanged(ClothFaceWearInfo, FaceWearSkelMesh, ESlotType::ClothFaceWear);
		break;
	case ESlotType::ClothEyeWear:
		ClothEyeWearInfo = _Info;
		HandleClothInfoChanged(ClothEyeWearInfo, EyeWearSkelMesh, ESlotType::ClothEyeWear);
		break;
	case ESlotType::ClothTopWear:
		ClothTopWearInfo = _Info;
		HandleClothInfoChanged(ClothTopWearInfo, TopWearSkelMesh, ESlotType::ClothTopWear);
		break;
	case ESlotType::ClothPants:
		ClothPantsInfo = _Info;
		HandleClothInfoChanged(ClothPantsInfo, PantsSkelMesh, ESlotType::ClothPants);
		break;
	case ESlotType::ClothGloves:
		ClothGlovesInfo = _Info;
		HandleClothInfoChanged(ClothGlovesInfo, GlovesSkelMesh, ESlotType::ClothGloves);
		break;
	case ESlotType::ClothBoots:
		ClothBootsInfo = _Info;
		HandleClothInfoChanged(ClothBootsInfo, BootsSkelMesh, ESlotType::ClothBoots);
		break;
	}
	
}

void ABRCharacter::OnRep_TotalAmmo()
{
	//탄약을 주울 때마다 In-Game 위젯에 표시되는 남은 탄약 수를 업데이트해 주어야 합니다.
	// IsLocallyControlled는 실제 플레이어에게만 이 로직을 실행하고, 다른 컴퓨터에 복제된 캐릭터에는 적용하지 않는다”는 의미
	HandleTotalAmmoChanged();
}

void ABRCharacter::HandleTotalAmmoChanged()
{
	if (!IsLocallyControlled())return;
	if (!PlayerInfo.Alive)return;
	if (IsValid(MyInventory->GetContainer(PlayerInfo.ActiveSlot))) {
		InGameWidget->UpdateAmmoCount(GetWeaponAmmoCurrentMagSize(), GetWeaponAmmoTotal());
	}
}

void ABRCharacter::SERVERTotalAmmo_Implementation(EPickupType _Type, int32 _NewCount)
{
	if (!HasAuthority()) return;

	switch (_Type)
	{
	case EPickupType::Ammo:
		TotalAmmo.Types[0] = _NewCount;
		break;
	case EPickupType::Ammo2:
		TotalAmmo.Types[1] = _NewCount;
		break;
	case EPickupType::Ammo3:
		TotalAmmo.Types[2] = _NewCount;
		break;
	case EPickupType::Ammo4:
		TotalAmmo.Types[3] = _NewCount;
		break;
	case EPickupType::Ammo5:
		TotalAmmo.Types[4] = _NewCount;
		break;
	case EPickupType::Ammo6:
		TotalAmmo.Types[5] = _NewCount;
		break;
	case EPickupType::Ammo7:
		TotalAmmo.Types[6] = _NewCount;
		break;
	case EPickupType::Ammo8:
		TotalAmmo.Types[7] = _NewCount;
		break;
	}
	HandleTotalAmmoChanged();
}

void ABRCharacter::OnRep_TotalGrenades()
{
	HandleTotalGrenadesChanged();
}

void ABRCharacter::HandleTotalGrenadesChanged()
{
	if (IsLocallyControlled() && IsValid(InGameWidget)) {
		InGameWidget->UpdateNadeCounts(TotalGrenades.FragGrenade, TotalGrenades.SmokeGrenade, TotalGrenades.StunGrenade);
	}
}

void ABRCharacter::SERVERTotalGrenades_Implementation(FName _GID, EPickupType _Type, int32 _NewCount)
{
	if (!HasAuthority()) return;
	
	if (_Type == EPickupType::FragGrenade)
		TotalGrenades.FragGrenade = _NewCount;
	else if(_Type == EPickupType::SmokeGrenade)
		TotalGrenades.SmokeGrenade = _NewCount;
	else if(_Type == EPickupType::StunGrenade)
		TotalGrenades.StunGrenade = _NewCount;
	if (_GID != TEXT(""))
		TotalGrenades.FragGID = _GID;
		HandleTotalGrenadesChanged();
	
}

void ABRCharacter::OnRep_IsPlayerDown()
{
	HandleIsPlayerDownChanged();
}

void ABRCharacter::HandleIsPlayerDownChanged()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (!IsLocallyControlled() || !IsValid(InGameWidget))return;
	UnAim();
	if (IsValid(RevivingPlayer))
	{
		SERVERBeingRevived(RevivingPlayer, false);
		InGameWidget->StopRevive();
		RevivingPlayer = nullptr;
	}
	InGameWidget->UpdateHealth(PlayerInfo.Health, IsPlayerDown);
}

void ABRCharacter::SERVERIsPlayerDown_Implementation(bool _PlayerDown)
{
	if (IsPlayerDown == _PlayerDown)return;

	
	IsPlayerDown = _PlayerDown;
	HandleIsPlayerDownChanged();
}

void ABRCharacter::OnRep_BeingRevived()
{
	HandleBeingRevivedChanged();
}

void ABRCharacter::HandleBeingRevivedChanged()
{
	if (!IsValid(InGameWidget))return;

	if (!BeingRevived) {
		InGameWidget->StopRevive();
		return;
	}
	if (IsPlayerDown)
		InGameWidget->StartRevive(ReviveDuration, false);
}
void ABRCharacter::SERVERBeingRevived_Implementation(ABRCharacter* _Character, bool _Reviving)
{

	if (IsValid(_Character)) {
		_Character->BeingRevived = _Reviving;
		_Character->HandleBeingRevivedChanged();
	}
}


void ABRCharacter::ServerOnReviveDone()
{
	if (!IsPlayerDown)return;
	SERVERBeingRevived(this, false);
	SERVERIsPlayerDown(false);
}

void ABRCharacter::OnRep_RepHealth()
{
	HandleRepHealthChanged();
}

void ABRCharacter::HandleRepHealthChanged()
{
	if (IsLocallyControlled())return;
	PlayerInfo.Alive = RepHealth > 0.f;
	PlayerInfo.Health = RepHealth;
}

void ABRCharacter::SERVERRepHealth_Implementation(float _Health)
{
	if (!HasAuthority()) return;

	if (_Health == RepHealth)return;
	RepHealth = _Health;
	HandleRepHealthChanged();
}

void ABRCharacter::OnRep_KillCount()
{
	HandleKillCountChanged();
}

void ABRCharacter::HandleKillCountChanged()
{
	if (IsLocallyControlled() && IsValid(InGameWidget)) {
		InGameWidget->UpdateKills(KillCount);
	}
}

void ABRCharacter::SERVERKillCount_Implementation(float _KillCount)
{
	if (!HasAuthority()) return;

	if (_KillCount == KillCount)return;
	KillCount = _KillCount;
	HandleKillCountChanged();
}

void ABRCharacter::SetKillCount(float _KillCount)
{
	KillCount = _KillCount;
	if (HasAuthority())
		HandleKillCountChanged();
}

void ABRCharacter::OnRep_Parachuting()
{
	HandleParachutingChanged();
}

void ABRCharacter::HandleParachutingChanged()
{
	if (Parachuting)
	{
		ifv(InGameWidget) {
			InGameWidget->HideBottomInfo();
		}
	}
	else {
		ifv(Parachute) {
			Parachute->Destroy();
			CameraBoom->SetRelativeLocation(CboomDefaultRelative);
			GetCharacterMovement()->GravityScale = 1.f;
		}
	}
}

void ABRCharacter::SERVERParachuting_Implementation(bool _Parachuting)
{
	if (!HasAuthority()) return;

	if (Parachuting == _Parachuting)return;
	Parachuting = _Parachuting;
	HandleParachutingChanged();
}

void ABRCharacter::OnRep_JumpingOffPlane()
{
	HandleJumpingOffPlaneChanged();
}

void ABRCharacter::HandleJumpingOffPlaneChanged()
{
	PlayerInfo.Movement = JumpingOffPlane ? EMovementState::Parachuting : EMovementState::Moving;
	if ((HasAuthority() || IsLocallyControlled()) && PlayerInfo.Movement == EMovementState::Parachuting)
		GetCharacterMovement()->AirControl = 1.f;
	
	if (IsLocallyControlled() && !Parachuting)
	{
		GetWorldTimerManager().SetTimer(OffPlaneTraceGroundHandle, this, &ABRCharacter::OffPlaneTraceGround, 1.0f, true);
	}
}

void ABRCharacter::SERVERJumpingOffPlane_Implementation(bool _JumpingOffPlane)
{

	if (!HasAuthority()) return;

	if (JumpingOffPlane == _JumpingOffPlane)return;
	JumpingOffPlane = _JumpingOffPlane;
	HandleJumpingOffPlaneChanged();
}

void ABRCharacter::ActiveSlotChanged(ESlotType _Slot)
{

	PlayerInfo.ActiveSlot = _Slot;
	if (!IsLocallyControlled())return;
	if (!IsValid(InGameWidget))return;
	if (PlayerInfo.ActiveSlot == ESlotType::Inventory) {
		InGameWidget->ClearActiveSlot();
		InGameWidget->ChangeCrosshair(ECrossHair::Dot, FVector2D(50.f, 50.f));
	}
	else {
		if (IsValid(MyInventory) && IsValid(MyInventory->GetContainer(_Slot))) {
			//if (!IsValid(MyInventory->GetContainer(_Slot)->GetSlotIndexItemWidget(0)))
				//return;
			//if (!MyInventory->GetContainer(_Slot)->GetSlotIndexItemWidget(0)->GetPickupInfo().EquipedImage)
				//return;

			InGameWidget->UpdateActiveSlot(MyInventory->GetContainer(_Slot)->GetSlotIndexItemWidget(0)->GetPickupInfo().EquipedImage, GetWeaponAmmoCurrentMagSize(), GetWeaponAmmoTotal());
			switch (PlayerInfo.ActiveSlot)
			{
			case ESlotType::Empty:
			case ESlotType::Inventory:
				InGameWidget->ChangeCrosshair(ECrossHair::Dot, FVector2D(50.f, 50.f));
				break;
			case ESlotType::Primary:
			case ESlotType::Secondary:
			case ESlotType::Pistol:
			case ESlotType::Melee:
			case ESlotType::Explosives:
				InGameWidget->ChangeCrosshair(ECrossHair::CrossHair, FVector2D(50.f, 50.f));
				break;
			}
		}
	}
}

void ABRCharacter::CLIENTConsumableClearSlot_Implementation()
{
	MyInventory->GetContainer(ESlotType::Consumable)->ClearSlot(0);
	CurrentConsumableSlot = EPickupType::None;
}
