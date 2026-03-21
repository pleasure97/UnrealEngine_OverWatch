// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "OWGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	static UOWGameUserSettings* Get();

	/* Gameplay Collection Tab */
	UFUNCTION()
	FString GetCurrentEnableAutomaticQueueRegistration() const { return CurrentEnableAutomaticQueueRegistration; }

	UFUNCTION()
	void SetCurrentEnableAutomaticQueueRegistration(const FString& InNewEnableAutomaticQueueRegistration) { CurrentEnableAutomaticQueueRegistration = InNewEnableAutomaticQueueRegistration;}

	UFUNCTION()
	FString GetCurrentLimitClientTransmissionRate() const { return CurrentLimitClientTransmissionRate; }

	UFUNCTION()
	void SetCurrentLimitClientTransmissionRate(const FString& InNewLimitClientTransmissionRate) { CurrentLimitClientTransmissionRate = InNewLimitClientTransmissionRate; }
	
	/* Gameplay Collection Tab Ends */

private:
	UPROPERTY(Config)
	FString CurrentEnableAutomaticQueueRegistration;

	UPROPERTY(Config)
	FString CurrentLimitClientTransmissionRate;
};
