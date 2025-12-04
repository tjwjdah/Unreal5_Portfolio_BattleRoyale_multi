// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Gameinfo.generated.h"
/*
FString : 언리얼엔진에서 제공되는 문자열 타입이다.
FString은 앞에 * 을 붙여주면 이 FString이 실제 가지고 있는 문자열의 주소를 의미한다.

__FUNCTION__ : 현재 이 로그를 출력하는 함수 이름을 의미한다.
__LINE__ : 현재 코드의 줄번호를 의미한다.
*/
DECLARE_LOG_CATEGORY_EXTERN(PP4, Log, All);
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define	LOG(Format, ...)	UE_LOG(PP4, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGSTRING(Str)		UE_LOG(PP4, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)
#define ifv(Var) if (IsValid(Var))
#define ifnv(Var) if (!IsValid(Var))
#define CV(Type, VarName, Object)                                                        \
    Type* VarName = Cast<Type>(Object);                                                  \
    //if (!VarName && GEngine)                                                             \
    {                                                                                    \
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,                          \
            FString::Printf(TEXT("[%s] Cast Failed: %s to %s"),                          \
            TEXT(__FUNCTION__), *GetNameSafe(Object), TEXT(#Type)));                     \
    }
void PrintViewport(float Time, const FColor& Color, const FString& Text);
const static FName SESSION_NAME = TEXT("BR");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");



UENUM(BlueprintType)
enum class EGameModeList : uint8
{
	Solo,
	Duo,
	Squad
};

UENUM(BlueprintType)
enum class EGameStatus : uint8
{
	Waiting,
	Starting,
	Started,
	Ended
};


UENUM(BlueprintType)
enum class EVehicleSeats : uint8
{
	Seat1,
	Seat2,
	Seat3,
	Seat4
};


UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Type1,
	Type2,
	Type3,
	Type4,
	Type5,
	Type6,
	Type7,
	Type8,
	Count UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECurrentActiveSlot : uint8
{
	None,
	Primary,
	Secondary,
	Pistol,
	Melee,
	Explosives,
};

UENUM(BlueprintType)
enum class EOnPickupAction : uint8
{
	None,
	Destroy,
	Hide
};

UENUM(BlueprintType)
enum class EPickupStatus : uint8
{
	Dropped,
	InInventory,
	Equiped,
	Destroyed,
	Hidden
};

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	None,
	Rifle,
	SMG,
	Shotgun,
	Pistol,
	GrenadeLauncher,
	MeleeWeapon,
	Ammo,
	Ammo2,
	Ammo3,
	Ammo4,
	Ammo5,
	Ammo6,
	Ammo7,
	Ammo8,
	WeaponAttachement,
	FragGrenade,
	SmokeGrenade,
	StunGrenade,
	Backpack,
	ClothHeadWear,
	ClothEyeWear,
	ClothFaceWear,
	ClothTopWear,
	ClothHoodie,
	ClothVeste,
	ClothGloves,
	ClothUnderwear,
	ClothPants,
	ClothBoots,
	Armor_HeadArmor,
	Armor_Body,
	HealingItem1,
	HealingItem2,
	HealingItem3,
	HealingItem4,
	HealingItem5
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	Empty,
	Inventory,      //1
	Primary, 		//2
	Secondary,		//3
	Pistol,			//4
	Melee,			//5
	Explosives,		//6
	WeaopnEnd,		//7
	HeadArmor,		//8
	BodyArmor,		//9
	ArmorEnd,		//10
	Backpack,		//11
	Consumable,		//12
	ClothHeadWear,	//13
	ClothFaceWear,	//14
	ClothEyeWear,	//15
	ClothTopWear,	//16
	ClothPants,		//17
	ClothGloves,	//18
	ClothBoots,		//19
	ClothEnd		//20
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Moving,
	InPlane,
	Parachuting,
	Driving
};

UENUM(BlueprintType)
enum class EWalkingState : uint8
{
	Walking,
	Normal,
	Sprinting
};

UENUM(BlueprintType)
enum class EHealingType : uint8
{
	Health,
	Boost,
	Water,
	Hunger
};


USTRUCT(BlueprintType)
struct FPickupInfos : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPickupType> Type = EPickupType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPickupStatus> Status = EPickupStatus::Dropped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> ActorRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanEquiped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EOnPickupAction> OnPickup = EOnPickupAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipSocket = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnEquipSocket = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnEquipSocket2 = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight=1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> InventoryImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> EquipedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ESlotType>> SupportedSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UniqueID=-1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PickupClass;
};


USTRUCT(BlueprintType)
struct FServerInventory
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ServerUniqueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> PickupRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPickupType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmorValue;
};

USTRUCT(BlueprintType)
struct FTotalAmmoCount
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Types;

	FTotalAmmoCount()
	{
		Types.Init(0, static_cast<int32>(EAmmoType::Count));
	}
};

USTRUCT(BlueprintType)
struct FTotalExplosives
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FragGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SmokeGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StunGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FragGID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SmokeGID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StunGID;
};

USTRUCT(BlueprintType)
struct FGameModeSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGameModeList> GameMode= EGameModeList::Solo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPlayersRequired_Start = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinimumPlayersRequired_Start = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ReadyUpTime = 15;

	//최소 플레이어로 강제 시작하는 최대 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxWaitTimeForceStartWithMinimumPlayers = 120;

	//최대 대기 시간에 봇으로 채우기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FillWithBotsOnMaxWaitTime;

	//아군 사격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FriendlyFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SquadPlayersCount = 5;

	//로비 캐릭터로 스폰 요청
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RequestSpawnWithLobbyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDeathMatch;

};

USTRUCT(BlueprintType)
struct FTeamInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APlayerController*> Players;

};

USTRUCT(BlueprintType)
struct FMinimapSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapturedMapSizeInWorldUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Zoom;
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamID=-1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Alive=true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health=100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isThirdPerson =true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotType> ActiveSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> PrimaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> SecondaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> PistolWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> MeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Explosive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementState> Movement= EMovementState::Moving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BackpackLV=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> BackpackRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaterPercent=0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostPercent=0.f;
};

USTRUCT(BlueprintType)
struct FCharacterArmorInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPickupInfos ArmorInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> ArmorSkelMesh;

};



USTRUCT(BlueprintType)
struct FCharacterClothInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEquip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> SkeletaAttach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldFollowMeshAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachToSocket=TEXT("None");
	
};


USTRUCT(BlueprintType)
struct FLeaningSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spine1=15.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spine2 = 5.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spine3 = 10.0;
};

USTRUCT(BlueprintType)
struct FLootItemSpawnChance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = TEXT("ItemID");	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance0To100 = 100.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BlockOtherSpawns;
};

USTRUCT(BlueprintType)
struct FZoneStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShrinkSpeed = 350.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PauseTime = 10.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerSec = 10.0;
};


USTRUCT(BlueprintType)
struct FLobbyPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PC;

};
