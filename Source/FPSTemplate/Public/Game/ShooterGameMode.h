// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ShooterGameModeBase.h"
#include "GameLiftServerSDK.h"
#include "ShooterGameMode.generated.h"

DECLARE_LOG_CATERGORY_EXTERN(LogShooterGameMode, Log, All); 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AShooterGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()
	
public:
	AShooterGameMode(); 

protected:
	virtual void BeginPlay() override; 

private:
	// Process Parameters needs to remain in scope for the lifetime of the app 
	FProcessParameters ProcessParameters;
	
	void InitGameLift(); 
};
