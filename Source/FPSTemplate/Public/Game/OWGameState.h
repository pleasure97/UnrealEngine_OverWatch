// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OWGameState.generated.h"

class UTeamCreationComponent; 
class UPlayerSpawningManagerComponent; 
class UMatchScoringComponent; 
class UPlayerPerformanceComponent; 
class UOWAbilitySystemComponent; 
struct FOWVerbMessage; 

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

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastReliableMessageToClients(const FOWVerbMessage Message); 

	/* Components - Team Creation */
	UPROPERTY()
	TObjectPtr<UTeamCreationComponent> TeamCreationComponent; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTeamCreationComponent> TeamCreationComponentClass;

	/* Components - Player Spawning */
	TObjectPtr<UPlayerSpawningManagerComponent> PlayerSpawningManagerComponent; 

	/* Components - Match Scoring */
	UPROPERTY()
	TObjectPtr<UMatchScoringComponent> MatchScoringComponent;

	/* Components - Player Performance */
	UPROPERTY()
	TObjectPtr<UPlayerPerformanceComponent> PlayerPerformanceComponent;

	/* Ability System Component */
	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> OWAbilitySystemComponent; 
};
