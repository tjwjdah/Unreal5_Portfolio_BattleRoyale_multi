// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "BRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	TArray<TObjectPtr<APawn>> Teammates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<APlayerController*> ToBeAdded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRCharacter> BRPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	TObjectPtr<APawn> MyPawnRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 TeamID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AZone> Zone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString CustomPlayerName;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAirPlane> PlanePawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool FollowPlane;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimerHandle TeammatesAddHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInGameWidget>	InGameWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInGameWidget> InGameWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UOnGameEndedWidget>	GameEndedWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOnGameEndedWidget> GameEndedWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UEscapeMenu>	EscapeMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEscapeMenu> EscapeMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCompassWidget> CompassWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCompassWidget> CompassWidget;
	/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAdminWidget> AdminWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAdminWidget> AdminWidget;
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UOnDeathMatchDeath> DeathMatchWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOnDeathMatchDeath> DeathMatchWidget;

public:
	ABRPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	void StopTeammatesAddHandle(){ GetWorldTimerManager().ClearTimer(TeammatesAddHandle); }

	void CheckAdded();

	void CanJumpOff();

	void AddNewTeammate(APlayerController* _Controller);

	void GameStarting(int32 _Time, bool _Starting);
	
	void GoLobby();

	bool HasTeammates();
	
	void PlaneSpawned(APawn* _PlanePawn);

	void SpectateTeam();

	void SpectateRandom();

	void CheckForSpawnRequest();
	
	void SetMinimapSettings(FMinimapSetting _Settings);

	ABRCharacter* GetBRPawn() { return BRPawn; }

	void SetMyPawnRef(APawn* _Pawn) { MyPawnRef = _Pawn; }

	APawn* GetMyPawnRef() { return MyPawnRef; }

	void SetTeamID(int32 _ID) { TeamID = _ID; }

	int32 GetTeamID() { return TeamID; }

	UInGameWidget* GetInGameWidget() { return InGameWidget; }

	const TArray<TObjectPtr<APawn>>& GetTeammates() { return Teammates; }

	void SetBRPawn(ABRCharacter* _Character) { BRPawn = _Character; }

public:
	UFUNCTION(Client, Reliable)
	void ShowInGameWidget();

	UFUNCTION(Client, Reliable)
	void LeaveServer();

	UFUNCTION(Client, Reliable)
	void OnDeathMatchDeath();

	UFUNCTION(Client, Reliable)
	void GameEnded(int32 _Rank);

	UFUNCTION(Client, Reliable)
	void UpdateKillFeed(const FText& _Killer, const FText& _Killed, const FText& _WeaponUsed);

	UFUNCTION(Client, Reliable)
	void ShowKillInfo(const FText& _VictimName);

	UFUNCTION(Server, Reliable)
	void RequestJumpOffPlane();

	UFUNCTION(Client, Reliable)
	void CLIENTJumpOffSuccess();

	void JumpOffSuccess();
	UFUNCTION(Server, Reliable)
	void ServerRequestSpawn(int32 _ClassID, const FString& _CustomName);
	
	UFUNCTION(Server, Reliable)
	void SERVERUpdateCustomName(const FString& _CustomPlayerName);

	UFUNCTION(Client, Reliable)
	void CLIENTInVehicleUpdate(bool _InVehicle, APawn* _Vehicle,ABRCharacter* _BRChar, EVehicleSeats _InVehicleSeat);
	
	UFUNCTION(Client, Unreliable)
	void CLIENTUpdateHealth(float _NewHealth, float _Water, float _Boost);

	UFUNCTION(Client, Reliable)
	void CLIENTPlayerInZoneStatus(bool _InZone);

	UFUNCTION(Client, Unreliable)
	void CLIENTShowHitmarker();


	
private:

	void Turn(float Scale);

	void LookUp(float Scale);

	void OpenCloseEscapeMenu();

	void Jump();
	
	void ShowMap();

	void CapsLock();
};
