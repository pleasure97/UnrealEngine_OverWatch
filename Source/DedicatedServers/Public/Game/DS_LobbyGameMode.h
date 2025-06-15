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

	void CheckAndStartLobbyCountTime();

	void CheckAndStopLobbyCountTime(); 

protected: 
	virtual void BeginPlay() override; 

	virtual void OnCountTimerFinished(ECountTimerType Type) override; 

	virtual void InitSeamlessTravelPlayer(AController* NewController) override; 

	virtual void Logout(AController* Exiting) override; 

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override; 

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;

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
	FCountTimerHandle LobbyCountTimer; 

	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters); 

	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage); 

	void AddPlayerInfoToLobbyState(AController* Player) const; 
	void RemovePlayerInfoFromLobbyState(AController* Player) const; 
};
