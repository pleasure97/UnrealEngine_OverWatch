// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OWGameState.generated.h"

class UTeamCreationComponent; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AOWGameState(); 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	virtual void PostInitializeComponents() override; 

	UPROPERTY()
	TObjectPtr<UTeamCreationComponent> TeamCreationComponent; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTeamCreationComponent> TeamCreationComponentClass;
};
