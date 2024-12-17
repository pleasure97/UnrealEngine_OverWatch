// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Lobby/LobbyPlayerInfo.h"
#include "LobbyState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChanged, const FLobbyPlayerInfo&, PlayerInfo); 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ALobbyState : public AInfo
{
	GENERATED_BODY()
	
public:
	ALobbyState(); 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const; 

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoAdded; 

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoRemoved; 

	void AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo); 
	void RemovePlayerInfo(const FString& Username); 
protected:
	UFUNCTION()
	void OnRep_LobbyPlayerInfo();

private:
	UPROPERTY(ReplicatedUsing = OnRep_LobbyPlayerInfo)
	FLobbyPlayerInfoArray PlayerInfoArray; 
	
};
