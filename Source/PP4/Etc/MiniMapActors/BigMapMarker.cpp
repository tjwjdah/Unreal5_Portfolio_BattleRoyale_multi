// Fill out your copyright notice in the Description page of Project Settings.


#include "BigMapMarker.h"

#include "../../Character/BRCharacter.h"

ABigMapMarker::ABigMapMarker()
{
	bAlwaysRelevant = true;
	SetReplicateMovement(true);
	SetReplicates(true);
}

void ABigMapMarker::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocal) {
		CV(ABRCharacter, Char, GetOwner());
		CV(ABRCharacter, Char2, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Char != Char2 && Char->GetTeamID() == Char2->GetTeamID()) {
			Char2->AddReplicatedMarker(GetColor(), this);
		}
	}
}
