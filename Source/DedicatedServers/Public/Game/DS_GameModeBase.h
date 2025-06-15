// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DSTypes.h"
#include "DS_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()
	
protected:
	void StartCountTimer(FCountTimerHandle& CountTimerHandle); 
	void StopCountTimer(FCountTimerHandle& CountTimerHandle); 
	void UpdateCountTimer(const FCountTimerHandle& CountTimerHandle); 
	virtual void OnCountTimerFinished(ECountTimerType Type); 
	
	void TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap); 

	void RemovePlayerSession(AController* Exiting); 
};
