// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "Styling/SlateColor.h"
#include "BRCharacter.generated.h"

UCLASS()
class PP4_API ABRCharacter : public ACharacter
{
	GENERATED_BODY()


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> TeammateIconWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PunchSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BootsSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GlovesSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> PantsSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> TopWearSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> EyeWearSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FaceWearSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadWearSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ArmorSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadArmorSkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> NadeLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPostProcessComponent> PostProcessOutOfZone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FPCamera;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> DynMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor CharacterColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector CboomDefaultRelative;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> CurrentHighlightedItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PickupTraceLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PickupSpawnDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BackpackSocketName;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothHeadData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothFaceData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothEyeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothTopData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothPantsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothGlovesData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPickupInfos ClothBootsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothHeadWearInfo")
	FCharacterClothInfo ClothHeadWearInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothFaceWearInfo")
	FCharacterClothInfo ClothFaceWearInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothEyeWearInfo")
	FCharacterClothInfo ClothEyeWearInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothTopWearInfo")
	FCharacterClothInfo ClothTopWearInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothPantsInfo")
	FCharacterClothInfo ClothPantsInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothGlovesInfo")
	FCharacterClothInfo ClothGlovesInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothBootsInfo")
	FCharacterClothInfo ClothBootsInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothArmorHeadInfo")
	FCharacterClothInfo ClothArmorHeadInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_ClothArmorBodyInfo")
	FCharacterClothInfo ClothArmorBodyInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxInventoryCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CurrentInventoryCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FServerInventory> ServerInventoryItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CurrentUniqueID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_TotalAmmo")
	FTotalAmmoCount TotalAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_TotalGrenades")
	FTotalExplosives TotalGrenades;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPickupType> CurrentConsumableSlot;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PunchTraceLen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PunchDamage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool Aiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float NormalFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AimSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AimFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AimPitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AimYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ScopeFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool UseScopeFOV;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing="OnRep_RepHealth")
	float RepHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ABRCharacter>> AddedTeammates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> LocalPlayerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ReviveDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABRCharacter> RevivingPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool SetCameraSpect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ParachuteClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> Parachute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ParachuteGravityScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float InAirForwardForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float InAirRightLForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> ScropWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_IsPlayerDown")
	bool IsPlayerDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABRCharacter> DownedPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_BeingRevived")
	bool BeingRevived;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMinimapWidget> MyMinimapClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMinimapWidget> MyMinimap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UOverviewMap> OverviewMapClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverviewMap> OverviewMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	FSlateColor MarkerColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABigMapMarker> OverviewMarkerClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABigMapMarker> OverviewMarker;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float NormalSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	TEnumAsByte<EWalkingState> WalkingState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool BtnWalkDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool BtnSprintDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool RotatingInPlace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float OnDeltaRotateInPlace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ExtraRotationChecks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool UseRotateInPlace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool BtnFwdDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool BtnRightDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool BtnLeftDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool GoingBackWards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	float RepLeaning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLeaningSettings LeaningSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	FRotator RepControl;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBRAnim> AnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> EquipMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> PunchMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MeleeAttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DrinkMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ThrowGrenadeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BandageMontage;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool MeleeAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Punching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_JumpingOffPlane")
	bool JumpingOffPlane;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo PlayerInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	int32 TeamID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool IsFiring;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool UsingItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	TEnumAsByte<ESlotType> RepActiveSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ESlotType> ChangeSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_KillCount")
	int32 KillCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool MinimapInitialized;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FCharacterArmorInfo BodyArmorInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FCharacterArmorInfo HeadArmorInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInventoryWidget>MyInventoryClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryWidget> MyInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInGameWidget> InGameWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInGameWidget> InGameWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UConsumeWidget> ItemUseWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UConsumeWidget> ItemUseWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UEquipInfoWidget> EquipInfoWClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipInfoWidget> EquipInfoW;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundCue> PunchSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool EnablePostProcessing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRPlayerController> ControllerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACharacterPreview> PrevCharacterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacterPreview> PrevCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPickupType> CurrentActiveExplosiveSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CurrentMicThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_Parachuting")
	bool Parachuting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EVehicleSeats> InVehicleSeat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool IsVehicleDriver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> VehicleRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool RepInVehicle;

	FTimerHandle OffPlaneTraceGroundHandle;
	FTimerHandle HandleWaterDepletionAndBoostHandle;
	FTimerHandle PlayerDownHealthDepletedHandle;
	bool bDrawDebug;
	bool bPunchDelay;
	bool bMeleeDelay;
	bool bEquipMontageOn;
	bool bBeginPlay;

protected:
	// Called when the game starts or when spawned

	virtual void OnConstruction(const FTransform& Transform);

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	virtual void Destroyed() override;
	public:
	void PressAttack();
	void ReleaseAttack();

	void Reload();

	void PrimarySlot();

	void SecondarySlot();

	void PistoleSlot();

	void MeleeSlot();

	void ExplosiveSlot();

	void ComsumablesSlot();

	void Unequip();

	void CharacterJump();

	void PressAim();
	void ReleaseAim();

	void PressSprint();
	void ReleaseSprint();

	void PressWalk();
	void ReleaseWalk();
	
	void CharacterCrouch();

	void PressLeanLeft();
	void ReleaseLeanLeft();

	void PressLeanRight();
	void ReleaseLeanRight();

	void Map();

	void ChangeCamera();

	void PressInteract();
	void ReleaseInteract();

	void Inventory();

	void Menu();

	void MoveForward(float Scale);
	void MoveRight(float Scale);

	UFUNCTION()
	void PunchEnd(UAnimMontage* _Montage, bool _Interrupted);
	UFUNCTION()
	void MeleeEnd(UAnimMontage* _Montage, bool _Interrupted);

	UFUNCTION()
	void EquipMontageEnd(UAnimMontage* _Montage, bool _Interrupted);
	UFUNCTION()
	void ConsumeInterrupted(UAnimMontage* _Montage, bool _Interrupted);

	UFUNCTION()
	void OnPunchBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,
		const FHitResult& SweepResult);
	void PunchDelayOn() { bPunchDelay = true; }
	void MeleeDelayOn() { bMeleeDelay = true; }
public:	
	// Sets default values for this character's properties
	ABRCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void WidgetSetPlayerID();

	void LookForPickups();

	void InteractWithWorld();

	void CanAutoEquiped(TArray<TEnumAsByte<ESlotType>>&SupportedSlots, bool& _CanAuto, ESlotType& _Equip, bool& _IsCloth);

	int32 GetWeaponAmmoTotal();

	int32 GetWeaponAmmoCurrentMagSize();

	int32 GetTotalAmmoCountAmmoType(EAmmoType _AmmoType);

	class UItemsContainer* GetContainerSlot(ESlotType _Slot);

	UItemsContainer* GetInfoContainerSlot(ESlotType _Slot);

	void ClientInventoryDragDrop(ESlotType _ContainerRecType, int32 _RecSlotIndex, ESlotType _ContainerSenderType, int32 _SenderSlotIndex);

	void UpdateMinimapAndTeammates();

	void ChangeMinimapSettings(UTexture2D* _Texture, float _CapturedAreaSizeInWorldUnits,float _Zoom);

	void RemoveActorOverviewmap(AActor* _Actor);

	void EquipSocket(AActor* _Pickup, FName _SocketName, bool _Attache);

	void ServerAttemptPickupItem(AActor* _Pickup);


	void InventoryDropItemRequest(ESlotType _ContainerType, int32 _UniqueServerID,int32 _Amount);

	AActor* SpawnPickup(TSubclassOf<AActor> _PickupClass, int32 _Count);

	void SetWeaponRefSlot(ESlotType _Type, AActor* _Actor);

	void UseItem(int32 _UniqueServerID);
		
	void CanReplaceEquip(AActor* _Pickup, bool& _CanReplace, ESlotType& _Slot);

	void DropPickupGround(AActor* _Pickup, FVector _NewLocation);

	void ServerReplaceEquip(AActor* _Pickup, ESlotType _Slot);

	FVector TraceToCrosshair(AActor* _Weapon, float _TraceLen);

	void DamageTaken(float _Damage, FString _Name, ACharacter* _Player, bool _DamageProjectile);

	void OnServerPlayerDown();

	void ServerInventoryExplosiveUsed(FName _ID);

	

	void ServerUseConsumable(int32 _UniqueServerID);

	void ServerPickupBackpack(AActor* _Pickup);

	void SERVERDropEquipItemsOnDeath();

	void OffPlaneTraceGround();

	bool CanAddedInventory(int32 _Count, float _Weight, float _MinusWeight, float& _NewWeigh);

	void GameStarted(bool _InPlane);

	bool SoloGame();

	bool IsListenServer() { return GetNetMode() == NM_ListenServer; }

	void AddPlaneMinimap(AActor* _PlaneActor);


	void UpdateArmor(ESlotType _Slot, float _NewArmor);

	void HandleWaterDepletionAndBoost();
	
	void SetEnableScope(bool _Enable);

	FVector GetPickupDropLocation(bool _Rand);

	bool GetStrafing();

	float GetFallDamage(float _Veclocity);

	void OnConsumableDroppedOverSlot(int32 _SlotIndex);

	void RefreshConsumableSlot(EPickupType _Type);

	bool InVehicle() { return PlayerInfo.Movement == EMovementState::Driving; }

	void ServerHandleRequestEnterVehicle(AActor* _Vehicle);

	void ServerHandleExitingVehicle(APawn* _Vehicle);

	void SetVehicleSeat(EVehicleSeats _Seat, APawn* _VehicleRef);

	bool IsGameStarted();

	
	FPickupInfos* GetClothDataSlot(ESlotType _Slot);

	FCharacterClothInfo GetClothInfoSlot(ESlotType _Slot);

	void OnEquipUpdatePreviewCharacter(ESlotType _Slot);

	void ClearAllCloth();

	void SwitchActiveExplosives();

	void OverviewMapClicked(FVector _Location, FKey _Button);

	bool UpdateGrenadeCount();

	FName GetCurrentActiveExplosiveSlotName();

	void UnAim();

	void SetupTeammateIcon(AActor* _Player);

	void ServerOnReviveDone();

	void OnSpectated();
	bool IsAlive() { return PlayerInfo.Alive; }

	void UpdateKillCount();

	void PlayerDownHealthDepleted();

	void WeaponFire(class AWeaponItem* _Weapon, bool _Loop);

	void SetWalkingState(EWalkingState _State);
	
	void AddReplicatedMarker(FSlateColor _Color, AActor* _Actor);

	void HandleBeginPlay();

	void CreatePreviewCharacter();

	void EventOnDropConsumables(ESlotType _ContainerRecType, int32 _RecSlotIndex, ESlotType _ContainerSenderType, int32 _SenderSlotIndex);

	void OnReviveProgressDone();
public:

	void InventoryUseItem(FKey _MouseButton,EPickupType _Type,int32 _Amount,int32 _ServerId);
		
	void ToggleInventory();

	bool HasWeapon(ESlotType _Slot);

	
	void ClearClothSlot(ESlotType _Slot);

	EPickupType GetPickupTypeAmmoType(EAmmoType _AmmoType);
	
	void SetInVehicle(bool _InVehicle);

	void BindInventoryevents();

	void ShowHideOverviewmap();

	void HandlePlayerDeath();
	
	UFUNCTION(NetMulticast, Reliable)
	void OnPickupEquipChanged(bool _Equip, AActor* _Pickup,
		FName _ToSocket, bool _Attach, bool _SetInvisile);


public:
	//MC
	UFUNCTION(NetMulticast, Reliable)
	void MCMulticastDoPlayerExitedVehicle();

	UFUNCTION(NetMulticast, Reliable)
	void MCActiveSlotChanged(ESlotType _Slot);
	void ActiveSlotChanged(ESlotType _Slot);
	UFUNCTION(NetMulticast, Reliable)
	void MCOnItemDrop(AActor* _Pickup, FVector _Location);

	UFUNCTION(NetMulticast, Reliable)
	void MCClientYouDied();

	UFUNCTION(NetMulticast, Unreliable)
	void MCPlaySound(USoundBase* _Sound, FVector _Location);

	UFUNCTION(NetMulticast, Reliable)
	void MCPlayerInVehicle(AActor* _Vehicle, FName _SeatName);

	UFUNCTION(NetMulticast, Unreliable)
	void MCPlayMontage(UAnimMontage* _Montage);
	
	UFUNCTION(NetMulticast, Reliable)
	void MCStopMontage();

	UFUNCTION(NetMulticast, Reliable)
	void MCPlayerExitedVehicle();

	UFUNCTION(NetMulticast, Reliable)
	void MCOpenParachute();
	void mcOpenParachute();
	//Server
	UFUNCTION(Server, Reliable)
	void SERVERRequestDoPlayerExitedVehicle();
	UFUNCTION(Server, Reliable)
	void SERVERRequestEnterVehicle(AActor* _Vehicle);

	UFUNCTION(Server, Reliable)
	void SERVERRequestExitVehicle();

	UFUNCTION(Server, Reliable)
	void SERVERPickupItem(AActor* _Pickup);
	UFUNCTION(Server, Reliable)
	void SERVERPickupBackPack(class ABackpackItem* _Pickup);
	UFUNCTION(Server, Reliable)
	void SERVERRequestLootBoxInteract(AActor* _LootBoxActor);
	UFUNCTION(Server, Reliable)
	void SERVERPrimarySecondarySwap();
	void ServerSwapPrimarySecondary();
	UFUNCTION(Server, Unreliable)
	void SERVEREquipInventoryMainSlot(ESlotType _Slot, int32 _UniqueID, int32 _SlotIndex);
	void ServerEquipInventoryMainSlot(ESlotType _Slot, int32 _UniqueID, int32 _SlotIndex);
	UFUNCTION(Server, Reliable)
	void SERVERDragDropEquipInventory(ESlotType _SlotType, int32 _UniqueID, int32 _SlotIndex);
	void ServerEquipInventory(ESlotType _SlotType, int32 _UniqueID, int32 _SlotIndex);

	UFUNCTION(Server, Reliable)
	void SERVERRequestOpenParachute();

	UFUNCTION(Server, Reliable)
	void SERVERRequestChangeActiveExplosive(EPickupType _Type);
	void ServerRequestChangeActiveExplosive(EPickupType _Type);

	UFUNCTION(Server, Unreliable)
	void SERVERPlayMontageToAll(UAnimMontage* _Montage);

	UFUNCTION(Server, Reliable)
	void SERVERFireWeapon();
	void ServerFireWeapon();

	UFUNCTION(Server, Reliable)
	void SERVERStopFiring();
	void ServerStopFiring();
	UFUNCTION(Server, Reliable)
	void SERVERThrowGrenade();
	void ServerThrowGrenade();

	UFUNCTION(Server, Unreliable)
	void SERVERSetWalkingState(EWalkingState _State);
		
	UFUNCTION(Server, Reliable)
	void SERVERPunchConnect(ACharacter* _Char, FVector _Location);
	void ServerPunchConnect(ACharacter* _Char, FVector _Location);

	UFUNCTION(Server, Reliable)
	void SERVERMeleeWeaponAtkConnect(ACharacter* _Char, FVector _Location);
	void ServerMeleeWeaponAtkConnect(ACharacter* _Char, FVector _Location);
	
	UFUNCTION(Server, Reliable)
	void SERVERReloadWeapon();
	void ServerReloadWeapon();

	UFUNCTION(Server, Reliable)
	void SERVERSetAiming(bool _Aiming);

	UFUNCTION(Server, Reliable)
	void SERVERChangeSlot(ESlotType _Slot);
	void ServerChangeSlot(ESlotType _Slot);

	UFUNCTION(Server, Reliable)
	void SERVERSetGoingBackWards(bool _GoingBackWards);

	UFUNCTION(Server, Reliable)
	void SERVERSetBtnFwdDown(bool _BtnFwdDown);

	UFUNCTION(Server, Reliable)
	void SERVERSetBtnRightDown(bool _BtnRight, bool _BtnLeft);

	UFUNCTION(Server, Reliable)
	void SERVERSetLeaning(float _Leaning);

	UFUNCTION(Server, Reliable)
	void SERVERAddUpdateMarker(FVector _Location);

	UFUNCTION(Server, Reliable)
	void SERVERRemoveMarker();


	UFUNCTION(Server, Reliable)
	void SERVERUpdateCameraMode(bool _IsThirdPerson);

	UFUNCTION(Server, Reliable)
	void SERVERDropInventoryItem(int32 _UniqueID, int32 _Count);
	void ServerDropInventoryItem(int32 _UniqueID, int32 _Count);

	UFUNCTION(Server, Reliable)
	void SERVERDropEquipItem(ESlotType _Slot);
	void ServerDropEquipItem(ESlotType _Slot);

	UFUNCTION(Server, Reliable)
	void SERVERUseItem(int32 _ServerID);

	UFUNCTION(Server, Reliable)
	void SERVEROnReviveDone(ABRCharacter* _RevivingPlayer);


	//Client

	UFUNCTION(Client, Reliable)
	void CLIENTConsumableClearSlot();

	UFUNCTION(Client, Reliable)
	void CLIENTUpdateInventoryItemCount(int32 _UniqueServerID,int32 _NewCount,float _NewCapacity);

	UFUNCTION(Client, Reliable)
	void CLIENTAddNewInventoryItem(FName  _PickupID,int32  _UniqueServerID, AActor* _Pickup, int32  _NewCapacity, int32  _SlotIndex, bool   _ClearSlot, int32  _Count);

	UFUNCTION(Client, Reliable)
	void CLIENTPickupEquip(FName  _PickupID, ESlotType _Slot,AActor* _Pickup, float _newWeight, int32 _SlotIndex, bool _Replaced);

	UFUNCTION(Client, Reliable)
	void CLIENTUpdateArmorValue(ESlotType _Slot, float _NewValue);

	UFUNCTION(Client, Unreliable)
	void CLIENTEquipWeaponDrop(ESlotType _Slot);

	UFUNCTION(Client, Unreliable)
	void CLIENTPrimarySecondarySwapped(AActor* _Primary,AActor* _Secondary,ESlotType _Slot);

	UFUNCTION(Client, Unreliable)
	void CLIENTUpdateHealth(float _NewHealth,float _Water, float _Boost);
	
	UFUNCTION(Client, Unreliable)
	void CLIENTShowHitmarker();

	UFUNCTION(Client, Reliable)
	void CLIENTBackPackEquip(float _MaxInvCapacity, int32 _BackpackLV, const FPickupInfos& _BackpackInfo);

	UFUNCTION(Client, Reliable)
	void CLIENTGameStarted(bool _InPlane);

	UFUNCTION(Client, Reliable)
	void ClientYouWin();

	UFUNCTION(Client, Reliable)
	void CLIENTFireStopUpdateAmmo(int32 _Ammo);
	void ClientFireStopUpdateAmmo(int32 _Ammo);

	UFUNCTION(Client, Reliable)
	void CLIENTPlayerInZoneStatus(bool _InZone);
	void PlayerInZoneStatus(bool _InZone);

public:
	void SetMarkerColor(FSlateColor _MarkerColor);

	void SetTeamID(int32 _TeamID) { TeamID = _TeamID; }

	int32 GetTeamID() { return TeamID; }

	void SetMinimapOwner(APawn* _Panw);

	ABRPlayerController* GetControllerRef() { return ControllerRef; }
	void SetControllerRef(ABRPlayerController* _Controller) { ControllerRef = _Controller; }

	UMinimapWidget* GetMinimapWidget() { return MyMinimap; }

	bool GetMinimapInitialized() { return MinimapInitialized; }

	UOverviewMap* GetOverviewMap() { return OverviewMap; }

	
	FPlayerInfo& GetPlayerInfo() { return PlayerInfo; }
	

	void SetJumpingOffPlane(bool _JumpingOffPlane) { 
		JumpingOffPlane = _JumpingOffPlane;
		HandleJumpingOffPlaneChanged();
	}

	ESlotType GetChangeSlot() { return ChangeSlot; }

	void SetEquipMontageOn(bool _On) { bEquipMontageOn = _On; }

	FRotator GetRepControl() { return RepControl; }

	ESlotType GetRepActiveSlot() { return RepActiveSlot; }

	bool GetIsFiring() { return IsFiring; }

	bool GetAiming() { return Aiming; }

	bool GetRotatingInPlace() { return RotatingInPlace; }

	float GetAimPitch() { return AimPitch; }

	float GetAimYaw() { return AimYaw; }

	bool GetBtnFwdDown() { return BtnFwdDown; }
	bool GetBtnRightDown() { return BtnRightDown; }
	bool GetBtnLeftDown() { return BtnLeftDown; }

	EWalkingState GetWalkingState() { return WalkingState; }

	bool GetParachuting() { return Parachuting; }

	float GetRepLeaning() { return RepLeaning; }

	FLeaningSettings GetLeaningSettings() { return LeaningSettings; }

	AActor* GetWeaponSlot(ESlotType _Slot);

	int32 GetNewUniqueID() { CurrentUniqueID += 1; return CurrentUniqueID; };

	FCharacterArmorInfo& GetBodyArmorInfo() { return BodyArmorInfo; }

	FCharacterArmorInfo& GetHeadArmorInfo() { return HeadArmorInfo; }

	bool GetIsVehicleDriver() { return IsVehicleDriver; }

	bool GetIsPlayerDown() { return IsPlayerDown; }

	bool GetEnablePostProcessing() { return EnablePostProcessing; }
	
	ACharacterPreview* GetPrevCharacter() { return PrevCharacter; }

private:
	UFUNCTION()
	void OnRep_ClothHeadWearInfo();
	UFUNCTION()
	void OnRep_ClothFaceWearInfo();
	UFUNCTION()
	void OnRep_ClothEyeWearInfo();
	UFUNCTION()
	void OnRep_ClothTopWearInfo();
	UFUNCTION()
	void OnRep_ClothPantsInfo();
	UFUNCTION()
	void OnRep_ClothGlovesInfo();
	UFUNCTION()
	void OnRep_ClothBootsInfo();
	UFUNCTION()
	void OnRep_ClothArmorHeadInfo();
	UFUNCTION()
	void OnRep_ClothArmorBodyInfo();

	void HandleClothInfoChanged(const FCharacterClothInfo& _ClothInfo, USkeletalMeshComponent* _Mesh, ESlotType _Slot);

	UFUNCTION(Server, Reliable)
	void SERVERClothInfo(ESlotType _Type, FCharacterClothInfo _Info);

	UFUNCTION()
	void OnRep_TotalAmmo();

	void HandleTotalAmmoChanged();

	UFUNCTION(Server, Reliable)
	void SERVERTotalAmmo(EPickupType _Type,int32 _NewCount);


	

	UFUNCTION()
	void OnRep_TotalGrenades();

	void HandleTotalGrenadesChanged();

	UFUNCTION(Server, Reliable)
	void SERVERTotalGrenades(FName _GID, EPickupType _Type, int32 _NewCount);


	UFUNCTION()
	void OnRep_IsPlayerDown();

	void HandleIsPlayerDownChanged();

	UFUNCTION(Server, Reliable)
	void SERVERIsPlayerDown(bool _PlayerDown);

	UFUNCTION()
	void OnRep_BeingRevived();

	void HandleBeingRevivedChanged();

	UFUNCTION(Server, Reliable)
	void SERVERBeingRevived(ABRCharacter* _Player, bool _Reviving);

	UFUNCTION()
	void OnRep_RepHealth();

	void HandleRepHealthChanged();

	UFUNCTION(Server, Reliable)
	void SERVERRepHealth(float _Health);

	UFUNCTION()
	void OnRep_KillCount();
	
	void HandleKillCountChanged();

	UFUNCTION(Server, Reliable)
	void SERVERKillCount(float _KillCount);

	void SetKillCount(float _KillCount);

	UFUNCTION()
	void OnRep_Parachuting();

	void HandleParachutingChanged();

	UFUNCTION(Server, Reliable)
	void SERVERParachuting(bool _Parachuting);

	UFUNCTION()
	void OnRep_JumpingOffPlane();

	void HandleJumpingOffPlaneChanged();

	UFUNCTION(Server, Reliable)
	void SERVERJumpingOffPlane(bool _JumpingOffPlane);
};
