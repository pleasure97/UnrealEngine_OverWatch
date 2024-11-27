// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ShooterGameMode.h"
#include "ShooterGameMode.h"

DEFINE_LOG_CATEGORY(LogShooterGameMode); 

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay(); 
#if WITH_GAMELIFT
	InitGameLift(); 
#endif
}

void AShooterGameMode::InitGameLift()
{
	UE_LOG(LogShooterGameMode, Log, TEXT("Initializing the Gamelift Server"));

	// Getting the module first. 
	FGameLiftServerSDKModule* GameLiftSDKModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK")); 

	// Define the server parameters for a GameLift Anywhere fleet. These are not needed for a Gamelift managed EC2 fleet. 
	FServerParameters ServerParameters; 

	// AuthToken returned from the "AWS Gamelift get-compute-auth-token" API. 
	// Note this will expire and require a new call to the API after 15 minutes. 
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), ServerParameters.m_authToken))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("AUTH_TOKEN : %s"), *ServerParameters.m_authToken); 
	}

	// The Host/Compute-name of the Gamelift Anywhere Instance. 
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), ServerParameters.m_hostId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("HOST_ID : %s"), *ServerParameters.m_hostId); 
	}

	// The Anywhere Fleet ID. 
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), ServerParameters.m_fleetId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("FLEET_ID : %s"), *ServerParameters.m_fleetId); 
	}

	// The WebSocket URL (GameliftServiceEndpoint)
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), ServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("WEBSOCKET_URL : %s"), *ServerParameters.m_webSocketUrl);
	}

	// The PID of the running process
	ServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId()); 

}
