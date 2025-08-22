// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "SolarRifle.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API USolarRifle : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBetweenShots = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SolarGauge = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSolarGauge = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargingSolarGaugeStandByTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDamage = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SolarGaugeRegeneration = 0.1538f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SolarGaugeRegenInterval = 0.1f;

	UFUNCTION(BlueprintCallable)
	void SetSolarGaugeWaitingTimerHandle(FTimerHandle InTimerHandle); 

private:
	void WaitForSolarGaugeRegeneration();

	void RegenerateSolarGaugeByTick();

	FTimerHandle SolarGaugeWaitingTimerHandle; 
	FTimerHandle SolarGaugeRegenTimerHandle; 
};
