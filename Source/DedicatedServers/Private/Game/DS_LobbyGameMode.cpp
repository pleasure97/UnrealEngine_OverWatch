// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_LobbyGameMode.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Kismet/GameplayStatics.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true; 
	LobbyStatus = ELobbyStatus::WaitingForPlayers; 
	MinPlayers = 1; 
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown; 
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountdown()
{
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer); 

	CheckAndStartLobbyCountdown(); 
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	CheckAndStartLobbyCountdown(); 
}


void ADS_LobbyGameMode::CheckAndStopLobbyCountdown()
{
	if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting); 

	CheckAndStopLobbyCountdown(); 
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay(); 

	InitGameLift(); 
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type); 

	if (Type == ECountdownTimerType::LobbyCountdown)
	{
		LobbyStatus = ELobbyStatus::SeamlessTravelling; 
		const FString MapName = DestinationMap.ToSoftObjectPath().GetAssetName(); 
		if (GIsEditor)
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap); 
		}
		else
		{
			GetWorld()->ServerTravel(MapName); 
		}
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


