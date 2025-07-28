// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "Message/OWMessageTypes.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "MatchScoringComponent.generated.h"

class AAssaultPoint; 
class UOWGamePhaseAbility; 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssaultPointRegistered, AAssaultPoint* AssaultPoint); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseRemainingTime, const FGameplayTag&, PhaseTag, float, RemainingCountdownTime); 

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class FPSTEMPLATE_API UMatchScoringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMatchScoringComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* Assault */
	void RegisterAssaultPoint(AAssaultPoint* AssaultPoint); 

	int32 GetTeamScore(int32 TeamId) const; 

	UFUNCTION(BlueprintCallable)
	void ScoreAssaultPoint(FOccupationInfo OccupationInfo);

	UPROPERTY(ReplicatedUsing=OnRep_AssaultPoints)
	TArray<AAssaultPoint*> AssaultPoints;

	FOnAssaultPointRegistered OnAssaultPointRegistered;

	/* Game Phase */
	UPROPERTY(BlueprintAssignable)
	FOnPhaseRemainingTime OnPhaseRemainingTime;

	UPROPERTY(ReplicatedUsing = OnRep_CountdownTime)
	float CountdownTime;

	FGameplayTag CurrentGamePhaseTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> FirstHeroSelectionGamePhaseAbility; 

	FOWGamePhaseTagDelegate FirstHeroSelectionGamePhaseDelegate;
	FOWGamePhaseTagDelegate FirstMatchPreparationDelegate;
	FOWGamePhaseTagDelegate FirstTeamOffensePhaseDelegate;
	FOWGamePhaseTagDelegate SecondHeroSelectionGamePhaseDelegate;
	FOWGamePhaseTagDelegate SecondMatchPreparationDelegate;
	FOWGamePhaseTagDelegate SecondTeamOffensePhaseDelegate;

protected:
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

private:
	/* Game Logic */
	UFUNCTION(BlueprintCallable)
	void ResetAllActivePlayers();

	UFUNCTION(BlueprintCallable)
	void HandleVictory(int32 TeamID); 

	UFUNCTION(BlueprintCallable)
	void ActivateMatchDecidedGameplayCue(FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters); 

	UFUNCTION(BlueprintCallable)
	void ClearMatchDecidedGameplayCue(); 

	UPROPERTY()
	int32 WinningTeamID = -1;

	FGameplayTag MatchDecidedTag;

	/* Assault */
	UFUNCTION()
	void OnRep_AssaultPoints(); 

	UPROPERTY()
	TEnumAsByte<EOccupationState> Team1OccupationState; 

	UPROPERTY()
	float Team1Progress = 0.f; 

	UPROPERTY()
	TEnumAsByte<EOccupationState> Team2OccupationState;

	UPROPERTY()
	float Team2Progress = 0.f; 

	/* Game Phase */
	UFUNCTION()
	void ConnectToGamePhase(); 

	UFUNCTION()
	void Countdown();

	UFUNCTION()
	void OnRep_CountdownTime(); 

	UFUNCTION()
	void HandleFirstHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration); 

	UFUNCTION()
	void HandleFirstMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	void GamePhaseStarted(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleFirstTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	FTimerHandle DelayTimerHandle; 
	FTimerHandle CountdownTimerHandle; 
};
