// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_LobbyGameMode.generated.h"

class UDS_GameInstanceSubsystem; 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()
	
public:
	ADS_LobbyGameMode(); 
	
	virtual void PostLogin(APlayerController* NewPlayer) override; 

	void CheckAndStartLobbyCountdown();

	void CheckAndStopLobbyCountdown(); 

protected: 
	virtual void BeginPlay() override; 

	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override; 

	virtual void InitSeamlessTravelPlayer(AController* NewController) override; 

	virtual void Logout(AController* Exiting) override; 

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override; 

	UPROPERTY()
	ELobbyStatus LobbyStatus; 

	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers; 

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap; 
	
private:

	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem; 

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer; 

	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters); 

};
