// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "LoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig)
class LOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
	
public:
	ULoadingScreenSettings(); 

	// Widget to load for loading screen. 
	UPROPERTY(config, EditAnywhere, meta=(MetaClass="/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenWidget; 

	// Z-order of loading screen widget in the viewport stack 
	UPROPERTY(config, EditAnywhere)
	int32 LoadingScreenZOrder = 10000; 
	
	// Interval in seconds beyond which loading screen is considered permanently hung 
	UPROPERTY(config, EditAnywhere, meta=(ForceUnits=s))
	float HeartbeatHangDuration = 0.f; 

	// Interval in seconds between each log of what is keeping a loading screen up 
	UPROPERTY(config, EditAnywhere, meta=(ForceUnits=s))
	float LogLoadingScreenHeartbeatInterval = 5.f; 
	
	UPROPERTY(Transient, EditAnywhere)
	bool HoldAdditionalSecondsEvenInEditor = false;

	UPROPERTY(config, EditAnywhere)
	bool ForceTickLoadingScreenEvenInEditor = true; 
};
