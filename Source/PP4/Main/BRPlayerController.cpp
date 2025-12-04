// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerController.h"
#include "BRGameInstance.h"
#include "BRGameState.h"
#include "BRPlayerState.h"
#include "PlaneGameMode.h"
#include "../Vehicles/Vehicle.h"
#include "../Character/BRCharacter.h"
#include "../Widget/InGameWidget.h"
#include "../Widget/EscapeMenu.h"
#include "../Widget/OnGameEndedWidget.h"
#include "../Widget/CompassWidget.h"
#include "../Widget/OnDeathMatchDeath.h"
#include "../Widget/AdminWidget.h"
#include "../Widget/Minimap/MinimapWidget.h"
#include "../Widget/Minimap/MinimapComponent.h"
#include "../Map/AirPlane.h"
#include "../World/Zone.h"
#include "../Vehicles/Vehicle.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

ABRPlayerController::ABRPlayerController()
{

	static ConstructorHelpers::FClassFinder<UInGameWidget> InGameW(TEXT("/Game/Widget/BPInGameWidget"));

	if (InGameW.Succeeded())
		InGameWidgetClass = InGameW.Class;

	static ConstructorHelpers::FClassFinder<UOnGameEndedWidget> GameEndedW(TEXT("/Game/Widget/BPOnGameEndedWidget"));

	if (GameEndedW.Succeeded())
		GameEndedWidgetClass = GameEndedW.Class;

	static ConstructorHelpers::FClassFinder<UEscapeMenu> EscapeMenuW(TEXT("/Game/Widget/BPEscapeMenu"));

	if (EscapeMenuW.Succeeded())
		EscapeMenuClass = EscapeMenuW.Class;

	static ConstructorHelpers::FClassFinder<UCompassWidget> CompassW(TEXT("/Game/Widget/BPCompassWidget"));

	if (CompassW.Succeeded())
		CompassWidgetClass = CompassW.Class;

	
	static ConstructorHelpers::FClassFinder<UCompassWidget> DeathMatchW(TEXT("/Game/Widget/BPOnDeathMatchDeath"));

	if (DeathMatchW.Succeeded())
		DeathMatchWidgetClass = DeathMatchW.Class;

	TeamID = -1;
	

}

void ABRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//폰이 아직 생성되지 않았을 때, 우리는 제어된 폰의 참조를 받아 복제된 배열에 추가하기 위해 스폰을 기다립니다.
	//When pawn hasn't spawned yet we wait for the spawn to get the reference of the controlled pawn and add it to the replicated array.
	if (HasAuthority()) {
		GetWorldTimerManager().SetTimer(TeammatesAddHandle, this, &ABRPlayerController::CheckAdded, 0.5f, true);
	}
	if (IsLocalController()) {
		CheckForSpawnRequest();
		CompassWidget = Cast<UCompassWidget>(CreateWidget(GetWorld(), CompassWidgetClass));
		CompassWidget->SetOwningPlayer(this);
		CompassWidget->AddToViewport();
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZone::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0) {
			Zone = Cast<AZone>(FoundActors[0]);
			Zone->SetPlayerController(this);
		}
	}
}


void ABRPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(ABRPlayerController, MyPawnRef);
	DOREPLIFETIME(ABRPlayerController, Teammates);
	
}

void ABRPlayerController::ShowInGameWidget_Implementation()
{
	//게임 내 위젯을 표시하기 위해 생성된 후 게임 모드에서 호출되는 이벤트
	if (IsValid(DeathMatchWidget)) {
		DeathMatchWidget->RemoveFromParent();
		DeathMatchWidget = nullptr;
	}
	InGameWidget = Cast<UInGameWidget>(CreateWidget(GetWorld(), InGameWidgetClass));
	InGameWidget->SetOwningPlayer(this);
	InGameWidget->AddToViewport();
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]() {
		CV(ABRGameState, State, GetWorld()->GetGameState());
		if (State->GetGameStatus() == EGameStatus::Waiting) {
			InGameWidget->WaitingForPlayers();
			//다음에 처리된 첫 번째 이벤트 디스패처를 놓칠 경우를 대비하여 플레이어 업데이트
			InGameWidget->UpdatePlayersAlive(State->GetPlayerLength());
		}
	}, 0.75f, false);
	if (IsValid(GetPawn())) {
		BRPawn = Cast<ABRCharacter>(GetPawn());
		ifv(BRPawn) {
			BRPawn->HandleBeginPlay();
			BRPawn->SetControllerRef(this);
		}
	}
	else {
		FTimerHandle TimerHandle2;
		GetWorldTimerManager().SetTimer(TimerHandle2, [this]() {
			if (IsValid(GetPawn())) {
				BRPawn = Cast<ABRCharacter>(GetPawn());
				ifv(BRPawn) {
					BRPawn->HandleBeginPlay();
					BRPawn->SetControllerRef(this);
				}
			}
		}, 0.2f, false);
	}
}

void ABRPlayerController::LeaveServer_Implementation()
{
	//연결이 거부되었거나 유사한 경우 클라이언트에게 로비로 이동하도록 요청하는 서버
	GoLobby();
}

void ABRPlayerController::OnDeathMatchDeath_Implementation()
{
	DeathMatchWidget = Cast<UOnDeathMatchDeath>(CreateWidget(GetWorld(), DeathMatchWidgetClass));
	DeathMatchWidget->SetOwningPlayer(this);
	DeathMatchWidget->AddToViewport();
	ifv(InGameWidget) {
		InGameWidget->RemoveFromParent();
		InGameWidget = nullptr;
	}
}

void ABRPlayerController::GameEnded_Implementation(int32 _Rank)
{

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this,_Rank]() {
		GameEndedWidget = Cast<UOnGameEndedWidget>(CreateWidget(GetWorld(), GameEndedWidgetClass));
		GameEndedWidget->SetOwningPlayer(this);
		GameEndedWidget->SetRank(_Rank);
		GameEndedWidget->AddToViewport();
		bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(GameEndedWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		}, 0.2f, false);
	
}

void ABRPlayerController::UpdateKillFeed_Implementation(const FText& _Killer, const FText& _Killed, const FText& _WeaponUsed)
{
	if (IsValid(InGameWidget)) {
		InGameWidget->AddKillFeed(_Killer, _Killed, _WeaponUsed);
	}

}
void ABRPlayerController::ShowKillInfo_Implementation(const FText& _VictimName)
{
	if (IsValid(InGameWidget)) {
		InGameWidget->ShowBottomInfo(FText::FromString(TEXT("You killed ")),_VictimName,FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)), FSlateColor(FLinearColor(1.0f, 0.6f, 1.0f)),3.0f);
	}
}

void ABRPlayerController::RequestJumpOffPlane_Implementation()
{
	//비행기 뛰어내리기 요청
	CV(ABRGameState, State, GetWorld()->GetGameState());
	if (State->GetCanJumpOffPlane()) {
		APlaneGameMode* Mode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
		if (Mode)
			Mode->RequestJumpOff(this);
	}
}





void ABRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Menu", IE_Pressed, this, &ABRPlayerController::OpenCloseEscapeMenu);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABRPlayerController::Jump);
	InputComponent->BindAction("ShowMap", IE_Pressed, this, &ABRPlayerController::ShowMap);
	InputComponent->BindAction("CapsLock", IE_Pressed, this, &ABRPlayerController::CapsLock);

	InputComponent->BindAxis("Turn", this,
		&ABRPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this,
		&ABRPlayerController::LookUp);
}

void ABRPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocalController() && FollowPlane && !IsValid(GetPawn())) {
		if (GetViewTarget() != PlanePawn) {
			SetViewTargetWithBlend(PlanePawn);
		}
		PlanePawn->SetViewRotation(FRotator(-GetInputAxisValue("LookUp"), GetInputAxisValue("Turn"), 0.f));
		SetControlRotation(PlanePawn->GetActorRotation());
	}
}

void ABRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABRCharacter* Char = Cast<ABRCharacter>(InPawn);
	ifv (Char) {
		Char->SetControllerRef(this);
		BRPawn = Char;
	}
}


void ABRPlayerController::CLIENTInVehicleUpdate_Implementation(bool _InVehicle, APawn* _Vehicle, ABRCharacter* _BRChar, EVehicleSeats _InVehicleSeat)
{
	BRPawn = _BRChar;
	_BRChar->SetVehicleSeat(_InVehicleSeat, _Vehicle);
	_BRChar->SetInVehicle(_InVehicle);
	_BRChar->SetMinimapOwner(_InVehicle ? _Vehicle : _BRChar);
	if (_InVehicle) {
		CV(AVehicle, Vehicle, _Vehicle);
		if (Vehicle)
			Vehicle->SetLocalOwner(_BRChar);
		InGameWidget->ClearActiveSlot();
	}


}

void ABRPlayerController::CLIENTUpdateHealth_Implementation(float _NewHealth, float _Water, float _Boost)
{
	ifv(InGameWidget) {
		InGameWidget->UpdateHealth(_NewHealth, false);
		FPlayerInfo& PlayerInfo = BRPawn->GetPlayerInfo();
		PlayerInfo.Health = _NewHealth;
		PlayerInfo.WaterPercent = _Water;
		PlayerInfo.BoostPercent = _Boost;
		InGameWidget->UpdateBoost(_Boost);
		InGameWidget->UpdateWater(_Water);

	}
}

void ABRPlayerController::CLIENTPlayerInZoneStatus_Implementation(bool _InZone)
{
	ifv(BRPawn) {
		BRPawn->PlayerInZoneStatus(_InZone);
	}
}

void ABRPlayerController::CLIENTShowHitmarker_Implementation()
{
	ifv(InGameWidget) {
		InGameWidget->ShowHitMarker();
	}
}

void ABRPlayerController::Turn(float Scale)
{
	if (!FollowPlane)AddYawInput(Scale);
	
}

void ABRPlayerController::LookUp(float Scale)
{
	if (!FollowPlane)AddPitchInput(Scale);
}

void ABRPlayerController::OpenCloseEscapeMenu()
{
	if (IsValid(EscapeMenu)) {
		if (EscapeMenu->IsVisible()) {
			EscapeMenu->RemoveFromParent();
			bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
		}
		else {
			EscapeMenu->AddToViewport();
			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(EscapeMenu->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
		}
	}
	else {
		EscapeMenu = Cast<UEscapeMenu>(CreateWidget(GetWorld(), EscapeMenuClass));
		EscapeMenu->SetOwningPlayer(this);
		OpenCloseEscapeMenu();
	}
}

void ABRPlayerController::Jump()
{
	if (auto* Char = Cast<ABRCharacter>(GetPawn()))
	{
		Char->CharacterJump();
		return;
	}
	if (FollowPlane) {
		CV(ABRGameState, State, GetWorld()->GetGameState());
		if (State->GetCanJumpOffPlane()) {
			RequestJumpOffPlane();
		}
	}
}

void ABRPlayerController::ShowMap()
{
	if(!IsValid(GetPawn()))return;
	ABRCharacter* Char = Cast<ABRCharacter>(MyPawnRef);
	Char->ShowHideOverviewmap();
}

void ABRPlayerController::CapsLock()
{
	/*
	if (HasAuthority()) {
		ifv(AdminWidget) {
			if (AdminWidget->IsVisible()) {
				AdminWidget->RemoveFromParent();
				bShowMouseCursor = false;
				FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
			}
			else {
				AdminWidget->AddToViewport();
				bShowMouseCursor = true;
				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(AdminWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);
			}
		}
		else{
		AdminWidget = Cast<UAdminWidget>(CreateWidget(GetWorld(), AdminWidgetClass));
		}
	}
	*/
}

void ABRPlayerController::CheckAdded()
{
	if (ToBeAdded.Num() > 0) {

		if (IsValid(ToBeAdded[0])) {
			if (IsValid(ToBeAdded[0]->GetPawn())) {
				Teammates.Add(ToBeAdded[0]->GetPawn());
				ToBeAdded.RemoveAt(0);
			}
		}
		else {
			ToBeAdded.RemoveAt(0);
			CheckAdded();
		}

	}
}

void ABRPlayerController::CanJumpOff()
{
	ifv(InGameWidget) {
		InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[SPACE]")), FText::FromString(TEXT("to jump!!")),
			FSlateColor(FLinearColor(0.322917f, 0.322917f, 0.322917f)), FSlateColor(FLinearColor(1.0f, 0.439738f, 0.0f)),0.f);
	}
}

void ABRPlayerController::AddNewTeammate(APlayerController* _Controller)
{
	ifv(_Controller->GetPawn())
		Teammates.Add(_Controller->GetPawn());
	else
		ToBeAdded.Add(_Controller);
}

void ABRPlayerController::GameStarting(int32 _Time, bool _Starting)
{
	if (IsValid(InGameWidget)) {
		InGameWidget->GameStarting(_Time, _Starting);
	}
}


void ABRPlayerController::GoLobby()
{
	UBRGameInstance* Instance = Cast<UBRGameInstance>(GetGameInstance());
	if(Instance)
		Instance->GoBackLobby();
}



bool ABRPlayerController::HasTeammates()
{
	for (int32 i = 0; i < Teammates.Num(); i++) {
		if (IsValid(Teammates[i])) {
			return true;
		}
	}
	return false;
}

void ABRPlayerController::PlaneSpawned(APawn* _PlanePawn)
{
	PlanePawn = Cast<AAirPlane>(_PlanePawn);
	ABRCharacter* Char =Cast<ABRCharacter>(MyPawnRef);
	if (IsValid(Char) && IsValid(PlanePawn)) {
		FollowPlane = true;
		Char->SetMinimapOwner(PlanePawn);
		SetControlRotation(PlanePawn->GetActorRotation());
		Char->AddPlaneMinimap(PlanePawn);
	}
}

void ABRPlayerController::SpectateTeam()
{
	if (IsValid(GameEndedWidget)) {
		ABRCharacter* Teammate= Cast<ABRCharacter>(Teammates[UKismetMathLibrary::RandomInteger(Teammates.Num())]);
		if (Teammate) {
			Teammate->OnSpectated();
			SetViewTargetWithBlend(Teammate, 0.1f, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
			GameEndedWidget->SetUpSpectating(Teammate);
		}
	}
}

void ABRPlayerController::SpectateRandom()
{
	if (IsValid(GameEndedWidget)) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABRCharacter::StaticClass(), FoundActors);
		ABRCharacter* RandomPlayer = Cast<ABRCharacter>(FoundActors[UKismetMathLibrary::RandomInteger(FoundActors.Num())]);
		if (RandomPlayer) {
			RandomPlayer->OnSpectated();
			SetViewTargetWithBlend(RandomPlayer, 0.1f, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
			GameEndedWidget->SetUpSpectating(RandomPlayer);

		}
	}
}

void ABRPlayerController::CheckForSpawnRequest()
{

	UBRGameInstance* Instance = Cast<UBRGameInstance>(GetGameInstance());
	if (Instance) {
		if (Instance->GetServerSetting().RequestSpawnWithLobbyCharacter) {
			ServerRequestSpawn(Instance->GetCharacterID(), Instance->GetCustomCharName());
		}
		else {
			if (Instance->GetCustomCharName() != TEXT(""))
				SERVERUpdateCustomName(Instance->GetCustomCharName());
		}
	}
}

void ABRPlayerController::ServerRequestSpawn_Implementation(int32 _ClassID, const FString& _CustomName)
{
	CustomPlayerName = _CustomName;
	ABRGameMode* GameMode = Cast<ABRGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->RequestSpawnWithID(_ClassID, this);
	}
}

void ABRPlayerController::SERVERUpdateCustomName_Implementation(const FString& _CustomPlayerName)
{
	CustomPlayerName = _CustomPlayerName;
	ABRPlayerState* State = Cast<ABRPlayerState>(PlayerState);
	if (State) {
		State->SetRepCustomName(CustomPlayerName);
	}
}


void ABRPlayerController::SetMinimapSettings(FMinimapSetting _Setting)
{
	if (!IsValid(GetPawn())) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, _Setting]() {
			SetMinimapSettings(_Setting);
			}, 0.5f, false);
	}
	ABRCharacter* Char = Cast<ABRCharacter>(GetPawn());
	if(!IsValid(Char))return;

	if (Char->GetMinimapInitialized()) 
			Char->ChangeMinimapSettings(_Setting.Texture, _Setting.CapturedMapSizeInWorldUnits, _Setting.Zoom);
	else 
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, _Setting]() {
		SetMinimapSettings(_Setting);
		}, 0.5f, false);
	}

}


void ABRPlayerController::CLIENTJumpOffSuccess_Implementation()
{
	FollowPlane = false;
	ifv(InGameWidget) {
		InGameWidget->ShowBottomInfo(FText::FromString(TEXT("[SPACE]")), FText::FromString(TEXT(" PARACHUTE!!")),
			FSlateColor(FLinearColor(0.322917f, 0.322917f, 0.322917f)), FSlateColor(FLinearColor(1.0f, 0.439738f, 0.0f)), 0.f);
		
		}
	ABRCharacter* Char = Cast<ABRCharacter>(MyPawnRef);
	ifv(Char) {
		Char->SetMinimapOwner(Char);
		Char->GetMinimapWidget()->GetMinimapComponent()->ToggleMyLocationVisibility();
	}
}

void ABRPlayerController::JumpOffSuccess()
{
	FollowPlane = false;
	CLIENTJumpOffSuccess();
}
