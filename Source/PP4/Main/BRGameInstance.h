// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Engine/GameInstance.h"
#include "../Character/LobbyCharacter/LobbyCharacter.h"
#include "../Character/BRCharacter.h"

#include "Misc/Optional.h"
#include "../World/Zone.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BRGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class PP4_API UBRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBRGameInstance();
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ULobbyScreen>	LobbyWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULobbyScreen> LobbyWidget;
	//게임 끝나고 로비로 되돌아왔을때 상태 유지하고 위젯있어야되니깐
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MaxPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsListen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, 
	meta = (AllowPrivateAccess = "true"))
	FGameModeSettings ServerSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString LobbyMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PathMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString GameMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool OpenMap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CharacterID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ALobbyCharacter>> LobbyCharacters;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABRCharacter>> MainCharacters;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString CustomCharName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> LobyCharacterPreview;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AZone> Zone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> PickupsDataTable;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString DesiredServerName;
	TOptional<FOnlineSessionSettings> PendingSessionSettings;
	int32 PendingControllerId;

public:
	virtual void Init() override;

	void ChangeLobbyClass(int32 _ID);

	bool IsDeathMatchGame();

	void DEDICATED_RefreshMap();

	void ShowLobbyWidget(APlayerController* _PlayerC);


	void HostGame(APlayerController* _PlayerController,int32 _PublicConnections,
		bool _bUseLAN, int32 _Mode,int32 _MaxPlayersRequired_Start,int32 _MinimumPlayersRequired_Start,
		bool _FillWithBotsOnMaxWaitTime,bool _FriendlyFire, FString _MapName,bool _OpenMap,bool _IsDeathMatch,FString _ServerName);

	void OnCreateSessionComplete(FName SessionName, bool Success);

	void GoBackLobby(); //GoBackToLobby
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void FindSession(APlayerController* _PlayerController, int32 _MaxResults, bool _UseLan);
	void OnFindSessionsComplete(bool Success);
	void Join(APlayerController* _PlayerController, const FOnlineSessionSearchResult& _Result);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSessionInternal(int32 ControllerId, FOnlineSessionSettings SessionSettings);
	

	void AddPlayerEntryLobby(int32 _ID, FText _PlayerNameVar, FText _PlayerPingVar);
	void UpdateEntryPing(int32 _ID, int32 _Ping);
	void RemoveEntry(int32 _ID);
	void SetPlayerName(FText _InText);
	void LoadGame(APlayerController* _SpecificPlayer);


public:
	TArray<TSubclassOf<ABRCharacter>>& GetMainCharacters() { return MainCharacters; }

	FGameModeSettings GetServerSetting() { return ServerSettings; }

	int32 GetCharacterID() { return CharacterID; }

	const FString& GetCustomCharName() { return CustomCharName; }
	void SetCustomCharName(FString _Name) { CustomCharName = _Name; }

	void SetLobyCharacterPreview(APawn* _Pawn) { LobyCharacterPreview = _Pawn; }

	const TArray<TSubclassOf<ALobbyCharacter>>& GetLobbyCharacters() { return LobbyCharacters; }

	void SetCharacterID(int32 _ID) { CharacterID = _ID; }

	AZone* GetZone();
	
	void SetZone(AZone* _Zone) { Zone = _Zone; }


	const FPickupInfos* GetItemData(FName RowName) const
	{
		if (!PickupsDataTable) return nullptr;
		return PickupsDataTable->FindRow<FPickupInfos>(RowName, TEXT(""));
	}
	const FPickupInfos* GetItemData(FName RowName,FString Context) const
	{
		if (!PickupsDataTable) return nullptr;
		return PickupsDataTable->FindRow<FPickupInfos>(RowName, Context);
	}

	IOnlineSessionPtr& GetSessionInterface() { return SessionInterface; }
};
