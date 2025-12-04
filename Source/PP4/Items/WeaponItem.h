// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AWeaponItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	AWeaponItem();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAmmoType> AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	int32 CurrentMagSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MaxMagSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundCue> FireStartSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> FireEndSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> MagOutSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> MagInSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") , Replicated)
	TObjectPtr<class ABRCharacter> WeaponOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double TraceLen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	double FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABullet> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool LoopFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> OnFireCamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimerHandle WeaponTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> PlayingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> EffectPlaying;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool isPlayingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Firing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool Ready;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> EmptyMagSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> ScopeWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ScopeFOV;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	virtual void Reload(int32 _AvailableAmmo, int32& _Used, int32& _RemainingAmmo);
	virtual  void StartFire();
	virtual  bool CanFire();
	virtual  bool GetLoopFire();
	virtual  void StopFire();
	virtual  TSubclassOf<UUserWidget> GetScopeWidget(float& _ScopeFOV);
	virtual  EAmmoType GetAmmoType();
	virtual int32 GetCurrentAmmo();
	virtual int32 GetMaxAmmo();
	virtual  bool GetCanReload();
	virtual UAnimMontage* GetReloadMontage();

	virtual bool Fire();
	int32 GetAmmoNeedFull();

	void PlayMagEmptySound();
	
	void PlayLoopSoundandEffect();
	
	void PlayCamShake();
	
	void StopSoundandEffect();

	
	void FireWeapon() {Fire();}

	UFUNCTION(Server, Reliable)
	void SetCurrentMagSize(int32 _CurrentMagSize);
	
	
	ABRCharacter* GetWeaponOwner() { return WeaponOwner; }


	
	int32 GetCurrentMagSize() { return CurrentMagSize; }
	
	float GetFireRate() { return FireRate; }

	FVector GetProjectileDestination();
public:

	UFUNCTION(NetMulticast, Reliable)
	virtual void MCPlaySoundandEffect();

	UFUNCTION(NetMulticast, Reliable)
	void MCStopSoundandEffect();

	UFUNCTION(NetMulticast, Reliable)
	void MCSetCurrentMagSize(int32 _CurrentMagSize);
	
	UFUNCTION(Server, Reliable)
	void SERVERSetWeaponOwner(ABRCharacter* _Char);
	
	UFUNCTION(Server, Reliable)
	void SERVERSetReady(bool _Ready);

protected:
	virtual void MCPlaySoundandEffect_Implementation();
};
