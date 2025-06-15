// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Types/DSTypes.h"
#include "DSPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTimerStateChangedDelegate, float, Time, ECountTimerDirection, Direction, ECountTimerType, Type); 

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADSPlayerController(); 

	virtual void ReceivedPlayer() override; 
	virtual void OnRep_PlayerState() override; 
	virtual void PostSeamlessTravel() override; 
	virtual void BeginPlay() override; 

	UFUNCTION(Client, Reliable)
	void Client_TimerUpdated(float CountTime, ECountTimerDirection Direction, ECountTimerType Type) const; 

	UFUNCTION(Client, Reliable)
	void Client_TimerStopped(float CountTime, ECountTimerDirection Direction, ECountTimerType Type) const;

	UFUNCTION(Client, Reliable)
	void Client_SetInputEnabled(bool bEnabled);

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerUpdated; 

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerStopped;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerSessionId;

	UPROPERTY(BlueprintReadOnly)
	FString Username; 

protected:
	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeOfRequest); 

	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeOfRequest); 
private:
	float SingleTripTime; 
	
};
