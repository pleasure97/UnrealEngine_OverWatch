// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "MatchScoringComponent.generated.h"

class AAssaultPoint; 
class UOWGamePhaseAbility; 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssaultPointRegistered, AAssaultPoint* AssaultPoint); 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UMatchScoringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMatchScoringComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void RegisterAssaultPoint(AAssaultPoint* AssaultPoint); 

	void ContestPoint(AAssaultPoint* AssaultPoint);

	int32 GetTeamScore(int32 TeamId) const; 

	UPROPERTY(ReplicatedUsing=OnRep_AssaultPoints)
	TArray<AAssaultPoint*> AssaultPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> HeroSelection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> PostMatch; 

	FOnAssaultPointRegistered OnAssaultPointRegistered;

protected:
	virtual void BeginPlay() override; 

private:
	UFUNCTION()
	void OnHeroSelectionStarted(UOWGamePhaseAbility* GamePhase); 

	void ResetAllActivePlayers();

	void ScorePoints(); 

	void GetPointStatus(int32 TeamID);

	void HandleVictory(int32 TeamID); 

	void ActivateMatchDecidedGameplayCue(FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters); 

	void ClearMatchDecidedGameplayCue(); 

	UFUNCTION()
	void OnRep_AssaultPoints(); 

	UPROPERTY()
	int32 WinningTeamID = -1; 

	FTimerHandle MatchPreparationTimerHandle; 
	FTimerHandle MatchInProgressTimerHandle; 
};
