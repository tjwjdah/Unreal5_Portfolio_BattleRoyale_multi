// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "ClothItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AClothItem : public APickupItem
{
	GENERATED_BODY()

	
public:
	AClothItem();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ShouldFollowMeshAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> ClothSeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName AttachSocket;

public:
	virtual void BeginPlay() override;

	bool GetShouldFollowMeshAnimation() { return ShouldFollowMeshAnimation; }

	USkeletalMesh* GetClothSeletalMesh() { return ClothSeletalMesh; }

	FName GetAttachSocket() { return AttachSocket; }
};
