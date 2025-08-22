// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/SolarRifle.h"

void USolarRifle::WaitForSolarGaugeRegeneration()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SolarGaugeWaitingTimerHandle);

		World->GetTimerManager().SetTimer(SolarGaugeRegenTimerHandle, this, &USolarRifle::RegenerateSolarGaugeByTick, SolarGaugeRegenInterval, true);
	}
}

void USolarRifle::SetSolarGaugeWaitingTimerHandle(FTimerHandle InTimerHandle)
{
	SolarGaugeWaitingTimerHandle = InTimerHandle;
}

void USolarRifle::RegenerateSolarGaugeByTick()
{
	if (UWorld* World = GetWorld())
	{
		if (SolarGauge >= MaxSolarGauge)
		{
			World->GetTimerManager().ClearTimer(SolarGaugeRegenTimerHandle); 
		}
		else
		{
			SolarGauge += SolarGaugeRegeneration; 
		}
	}
}
