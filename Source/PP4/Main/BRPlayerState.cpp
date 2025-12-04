// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerState.h"
#include "Net/UnrealNetwork.h"

void ABRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME_CONDITION(ABRPlayerState, kills, COND_OwnerOnly);
	DOREPLIFETIME(ABRPlayerState, RepCustomName);

}