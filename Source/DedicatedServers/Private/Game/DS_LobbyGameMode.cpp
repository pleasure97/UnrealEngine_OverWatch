// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_LobbyGameMode.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSPlayerController.h"
#include "GameLiftServerSDK.h"
#include "Game/DSGameState.h"
#include "Lobby/LobbyState.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true; 
	LobbyStatus = ELobbyStatus::WaitingForPlayers; 
	MinPlayers = 1; 
	LobbyCountTimer.Type = ECountTimerType::LobbyCountup; 
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountTime()
{
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::PreparingSeamlessTravel;
		StartCountTimer(LobbyCountTimer);
	}
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer); 

	CheckAndStartLobbyCountTime(); 

	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::PostLogin for %s"), *NewPlayer->GetName()); 
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	CheckAndStartLobbyCountTime(); 

	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		AddPlayerInfoToLobbyState(NewController); 
	}

	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitSeamlessTravelPlayer for %s"), *NewController->GetName());
}


void ADS_LobbyGameMode::CheckAndStopLobbyCountTime()
{
	if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::PreparingSeamlessTravel)
	{
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountTimer(LobbyCountTimer);
	}
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting); 

	CheckAndStopLobbyCountTime(); 

	RemovePlayerSession(Exiting); 

	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		RemovePlayerInfoFromLobbyState(Exiting); 
	}

	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::Logout for %s"), *Exiting->GetName());
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage); 

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId")); 
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username")); 

	TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage); 

	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::PreLogin - PlayerSessionId : %s, Username : %s"), *PlayerSessionId, *Username);
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal); 

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId")); 
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username")); 

	if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->Username = Username;
	}
	
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		AddPlayerInfoToLobbyState(NewPlayerController); 
	}

	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitNewPlayer - PlayerSessionId : %s, Username : %s"), *PlayerSessionId, *Username);

	return InitializedString; 
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("Player Session ID or Username is invalid."); 
		return;
	}

#if WITH_GAMELIFT
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionRequest; 
	DescribePlayerSessionRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId)); 

	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionRequest); 
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("DescribePlayerSession failed."); 
		return; 
	}

	const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult(); 
	int32 Count = 0; 
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count); 
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSessions failed."); 
		return; 
	}

	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i]; 
		if (!Username.Equals(PlayerSession.GetPlayerId())) continue; 
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; fail PreLogin."), *Username); 
			return; 
		}

		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId)); 
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to accept player session for %s"), *Username); 
	}
#endif 
}

void ADS_LobbyGameMode::AddPlayerInfoToLobbyState(AController* Player) const
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Player); 
	ADSGameState* DSGameState = GetGameState<ADSGameState>(); 
	if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(DSPlayerController))
	{
		FLobbyPlayerInfo PlayerInfo(DSPlayerController->Username); 
		DSGameState->LobbyState->AddPlayerInfo(PlayerInfo); 
	}
}

void ADS_LobbyGameMode::RemovePlayerInfoFromLobbyState(AController* Player) const
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Player); 
	ADSGameState* DSGameState = GetGameState<ADSGameState>(); 
	if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(DSPlayerController))
	{
		DSGameState->LobbyState->RemovePlayerInfo(DSPlayerController->Username); 
	}
}


void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay(); 

	InitGameLift(); 
}

void ADS_LobbyGameMode::OnCountTimerFinished(ECountTimerType Type)
{
	Super::OnCountTimerFinished(Type); 

	if (Type == ECountTimerType::LobbyCountdown)
	{
		StopCountTimer(LobbyCountTimer); 
		LobbyStatus = ELobbyStatus::SeamlessTravelling; 
		TrySeamlessTravel(DestinationMap); 
	}
}

void ADS_LobbyGameMode::InitGameLift()
{

	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters; 
			SetServerParameters(ServerParameters); 
			DSGameInstanceSubsystem->InitGameLift(ServerParameters); 
		}
	}
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	// AuthToken returned from the "AWS Gamelift get-compute-auth-token" API. 
	// Note this will expire and require a new call to the API after 15 minutes. 
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN : %s"), *OutServerParameters.m_authToken);
	}

	// The Host/Compute-name of the Gamelift Anywhere Instance. 
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID : %s"), *OutServerParameters.m_hostId);
	}

	// The Anywhere Fleet ID. 
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID : %s"), *OutServerParameters.m_fleetId);
	}

	// The WebSocket URL (GameliftServiceEndpoint)
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL : %s"), *OutServerParameters.m_webSocketUrl);
	}

	// The PID of the running process
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogDedicatedServers, Log, TEXT("PID : %s"), *OutServerParameters.m_processId);
}

