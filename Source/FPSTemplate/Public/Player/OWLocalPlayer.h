// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/TeamInterface.h"
#include "OWLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWLocalPlayer : public ULocalPlayer, public ITeamInterface
{
	GENERATED_BODY()
	
public:
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override; 
	virtual void InitOnlineSession() override; 
	
protected:
	void OnPlayerControllerChanged(APlayerController* NewController); 

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam); 

private:
	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPlayerController;
};
