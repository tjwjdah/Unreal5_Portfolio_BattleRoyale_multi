// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"

#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class ALobbyCharacter>> LobbyCharacterClass;

public:
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/*
	PreLogin
	아직 연결 허용 여부 체크 단계(밴 여부, 슬롯 꽉 찼는지 등)
	아직 PlayerController도 없음.
	Login
	접속이 허가되면 PlayerController 가 생성되고 이 플레이어가 서버에 등록됨.
	Return 값으로 그 플레이어의 APlayerController* 가 나옴.
	PostLogin(← 우리가 말하는 그 함수)
	위에서 생성된 NewPlayerController 를 인자로 받아 호출.
	이 시점부터는 “이 플레이어는 서버에 공식적으로 들어왔다”고 보면 됨.
	*/

	TSubclassOf<ALobbyCharacter> GetCharacterClassID(int32 _ID = 0) {return LobbyCharacterClass[_ID];}
};
