// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class PP4_API APickupItem : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPickupComponent> PickupComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	int32 NewCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTransform StaticDefTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTransform SkelDefTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_PickupStatus")
	TEnumAsByte<EPickupStatus> PickupStatus;
	

public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* GetStaticMesh() { return StaticMesh; }
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




public:
	UFUNCTION()
	void OnRep_PickupStatus();

	UFUNCTION(Server, Reliable)
	void SERVERSetNewCount(int32 _Count);

	void SetCollisionT(ECollisionEnabled::Type _Type, bool _Hide, bool _SimulPhysics);

	void PickupNotEquip();

	bool SetPickupStatus(EPickupStatus _Status);

	void UpdateStatus();

	USceneComponent* GetMeshComponentRef();

	void MeshComponenReset();

	virtual FPickupInfos& GetInfo();
	virtual FText GetNameAndCount();

	virtual void UpdateCount(int32 _NewCount);

	virtual FTransform GetDefaultTransform();
};
