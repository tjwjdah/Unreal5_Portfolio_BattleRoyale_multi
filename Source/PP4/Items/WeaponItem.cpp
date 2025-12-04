// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"
#include "Bullet.h"

#include "../Character/BRCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
AWeaponItem::AWeaponItem()
{
	AmmoType = EAmmoType::Count;
	MaxMagSize = 32;
	LoopFire = true;
	MuzzleSocketName = TEXT("Muzzle");
	Ready = true;

	PlayingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayingSound"));
	EffectPlaying = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectPlaying"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Reload(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/Reload"));

	if (Reload.Succeeded())
		ReloadMontage = Reload.Object;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	OnFireCamShakeClass(TEXT("/Game/Item/Guns/BPWeaponFireCamShake"));

	if (OnFireCamShakeClass.Succeeded())
		OnFireCamShake = OnFireCamShakeClass.Class;
}
void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();


}
void AWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetWeaponOwner())
		PrintViewport(5.f, FColor::Red, FString::Printf(TEXT("%d"),CurrentMagSize));
}
void AWeaponItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponItem, WeaponOwner);
	DOREPLIFETIME(AWeaponItem, CurrentMagSize);
	DOREPLIFETIME(AWeaponItem, Ready);
}
void AWeaponItem::Reload(int32 _AvailableAmmo, int32& _Used, int32& _RemainingAmmo)
{
	
	if (_AvailableAmmo >= GetAmmoNeedFull()) {
		int32 AmmoNeed;
		AmmoNeed = GetAmmoNeedFull();
		CurrentMagSize = MaxMagSize;
		MCSetCurrentMagSize(MaxMagSize);
		_Used = AmmoNeed;
		_RemainingAmmo = _AvailableAmmo - AmmoNeed;
	}
	else {
		CurrentMagSize = CurrentMagSize + _AvailableAmmo;
		MCSetCurrentMagSize(CurrentMagSize);
		_Used = _AvailableAmmo;
		_RemainingAmmo = 0;
	}
	
}

void AWeaponItem::StartFire()
{
	Firing = true;
	GetWorldTimerManager().ClearTimer(WeaponTimerHandle);
	WeaponTimerHandle.Invalidate();
	Fire();
	if (LoopFire) {
		GetWorldTimerManager().SetTimer(WeaponTimerHandle, this, &AWeaponItem::FireWeapon, FireRate > 0 ? FireRate : 1.f, true);
		if (IsValid(FireStartSound))
			MCPlaySoundandEffect();
	}
	else {
		Firing = false;
		if (IsValid(FireStartSound))
			MCPlaySoundandEffect();
		Ready = false;
		SERVERSetReady(false);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this] {
			Ready = true;
			SERVERSetReady(true);
			}, FireRate, false);
	}
	
}

bool AWeaponItem::CanFire()
{

	if (CurrentMagSize > 0 && IsValid(WeaponOwner) && Ready)
		return true;
	else 
		return false;

}

bool AWeaponItem::GetLoopFire()
{
	return LoopFire;
}

void AWeaponItem::StopFire()
{
	GetWorldTimerManager().ClearTimer(WeaponTimerHandle);
	WeaponTimerHandle.Invalidate();
	MCStopSoundandEffect();
}

TSubclassOf<UUserWidget> AWeaponItem::GetScopeWidget(float& _ScopeFOV)
{
	if (ScopeWidget)
	{
		_ScopeFOV = ScopeFOV;
		return ScopeWidget;
	}
	else return nullptr;
}

EAmmoType AWeaponItem::GetAmmoType()
{
	return AmmoType;
}

int32 AWeaponItem::GetCurrentAmmo()
{
	return CurrentMagSize;
}

int32 AWeaponItem::GetMaxAmmo()
{
	return MaxMagSize;
}

bool AWeaponItem::GetCanReload()
{
	return CurrentMagSize < MaxMagSize;
}

UAnimMontage* AWeaponItem::GetReloadMontage()
{
	return ReloadMontage;
}

int32 AWeaponItem::GetAmmoNeedFull()
{
	return MaxMagSize-CurrentMagSize;
}

void AWeaponItem::PlayMagEmptySound()
{
	if (!IsValid(EmptyMagSound) || CurrentMagSize>0)return;
	PlayingSound = UGameplayStatics::SpawnSoundAttached(EmptyMagSound, GetMeshComponentRef(), MuzzleSocketName,FVector::ZeroVector,EAttachLocation::SnapToTarget,true);
}

void AWeaponItem::PlayLoopSoundandEffect()
{
	isPlayingSound = true;
	if (!IsValid(FireStartSound))
		return;
	PlayingSound=UGameplayStatics::SpawnSoundAttached(FireStartSound, GetMeshComponentRef(),MuzzleSocketName);
	if (!IsValid(MuzzleFlash))
		return;
	EffectPlaying= UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GetMeshComponentRef(), MuzzleSocketName);
}

void AWeaponItem::PlayCamShake()
{
	
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), OnFireCamShake, UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation(), 25.f, 50.f, 1.f);
	
}

void AWeaponItem::StopSoundandEffect()
{
	isPlayingSound = false;
	if (LoopFire) {
		if (IsValid(PlayingSound))
			PlayingSound->FadeOut(0.1f, 0.f);
	}
	if (IsValid(FireEndSound)) 
		UGameplayStatics::SpawnSoundAttached(FireEndSound, GetMeshComponentRef(), MuzzleSocketName)->FadeIn(0.1);
	if (LoopFire) {
		if (IsValid(EffectPlaying))
		{
			EffectPlaying->DestroyComponent();
			EffectPlaying = nullptr;
		}
	}
	else {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this] {
			if (IsValid(EffectPlaying))
			{
				EffectPlaying->DestroyComponent();
				EffectPlaying = nullptr;
			}
			}, 0.1f, false);

	}
}

void AWeaponItem::SetCurrentMagSize_Implementation(int32 _CurrentMagSize)
{
	CurrentMagSize = _CurrentMagSize;
}

bool AWeaponItem::Fire()
{
	if (!CanFire())
		return false;

	FVector Loc = GetMeshComponentRef()->GetSocketLocation(MuzzleSocketName);
	FTransform Trans2; Trans2.SetLocation(Loc);
	Trans2.SetRotation(UKismetMathLibrary::FindLookAtRotation(Loc, GetProjectileDestination()).Quaternion());
	ABullet* Bullet = GetWorld()->SpawnActorDeferred<ABullet>(ProjectileClass, Trans2, nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Bullet->SetOwner(WeaponOwner);
	Bullet->SetWeapon(this);
	Bullet->FinishSpawning(Trans2);
	CurrentMagSize -= 1;
	SetCurrentMagSize(CurrentMagSize);
	if (CurrentMagSize > 0)
		return true;
	CurrentMagSize = 0;
	SetCurrentMagSize(CurrentMagSize);
	StopFire();
	return true;
}

FVector AWeaponItem::GetProjectileDestination()
{
	return WeaponOwner->TraceToCrosshair(this, TraceLen);
}

void AWeaponItem::MCSetCurrentMagSize_Implementation(int32 _CurrentMagSize)
{
	CurrentMagSize = _CurrentMagSize;
}

void AWeaponItem::SERVERSetWeaponOwner_Implementation(ABRCharacter* _Char)
{ 
	WeaponOwner = _Char; 
}

void AWeaponItem::SERVERSetReady_Implementation(bool _Ready)
{
	Ready = true; 
}

void AWeaponItem::MCPlaySoundandEffect_Implementation()
{
	isPlayingSound = true;
	if (!IsValid(WeaponOwner))return;

	if (!WeaponOwner->IsLocallyControlled() && !IsRunningDedicatedServer())
		PlayLoopSoundandEffect();
}

void AWeaponItem::MCStopSoundandEffect_Implementation()
{
	isPlayingSound = false;
	if (!IsValid(WeaponOwner))return;

	if (!WeaponOwner->IsLocallyControlled() && !IsRunningDedicatedServer())
		StopSoundandEffect();
}

