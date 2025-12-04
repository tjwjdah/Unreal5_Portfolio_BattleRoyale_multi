// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Parent_PointOfInterest.h"
#include "BigMapMarker.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABigMapMarker : public AParent_PointOfInterest
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsLocal;
	
	
public:
	ABigMapMarker();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void SetLocal(bool _Local) { IsLocal = _Local; }


};
