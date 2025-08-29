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
class AOWPlayerStart; 
class UGameplayEffect; 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssaultPointRegistered, AAssaultPoint* AssaultPoint); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseRemainingTime, const FGameplayTag&, PhaseTag, float, RemainingCountdownTime); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOccupationProgressDecided, const FGameplayTag&, PhaseTag, float, NewOccupationProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAssaultScoreDecided, int32, TeamID, int32, NewAssaultScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinningTeamDecided, int32, WinningTeamID);


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

	UFUNCTION(BlueprintCallable)
	AAssaultPoint* GetActiveAssaultPoint() const; 

	int32 GetTeamScore(int32 TeamId) const; 

	float GetTeamOccupationProgress(int32 TeamId) const; 

	void SetTeamOccupationProgress(int32 TeamId, float OccupationProgress); 

	float GetTeamRemainingTime(int32 TeamId) const; 

	int32 GetWinningTeamID() const; 

	TArray<AOWPlayerStart*> GetAllPlayerStarts() const;

	UFUNCTION(BlueprintCallable)
	void ScoreAssaultPoint(FOccupationInfo OccupationInfo);

	UPROPERTY(ReplicatedUsing=OnRep_AssaultPoints)
	TArray<AAssaultPoint*> AssaultPoints;

	FOnAssaultPointRegistered OnAssaultPointRegistered;
	FOnOccupationProgressDecided OnOccupationProgressDecided; 
	FOnAssaultScoreDecided OnAssaultScoreDecided; 
	FOnWinningTeamDecided OnWinningTeamDecided; 

	/* Game Phase */
	UPROPERTY(BlueprintAssignable)
	FOnPhaseRemainingTime OnPhaseRemainingTime;

	UPROPERTY(ReplicatedUsing = OnRep_CountdownTime)
	float CountdownTime;

	UPROPERTY(Replicated)
	FGameplayTag CurrentGamePhaseTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> FirstHeroSelectionGamePhaseAbility; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> SwitchInningGamePhaseAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> PostMatchGamePhaseAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> MatchDecidedGameplayEffect; 

	FOWGamePhaseTagDelegate FirstHeroSelectionGamePhaseDelegate;
	FOWGamePhaseTagDelegate FirstMatchPreparationDelegate;
	FOWGamePhaseTagDelegate FirstTeamOffensePhaseDelegate;
	FOWGamePhaseTagDelegate SwitchInningPhaseDelegate; 
	FOWGamePhaseTagDelegate SecondHeroSelectionGamePhaseDelegate;
	FOWGamePhaseTagDelegate SecondMatchPreparationDelegate;
	FOWGamePhaseTagDelegate SecondTeamOffensePhaseDelegate;

protected:
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

private:
	/* Game Logic */
	UFUNCTION(BlueprintCallable)
	void HandleVictory(int32 TeamID); 

	UFUNCTION(BlueprintCallable)
	void ActivateMatchDecidedGameplayCue(FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters); 

	UFUNCTION(BlueprintCallable)
	void ClearMatchDecidedGameplayCue(); 

	UPROPERTY(ReplicatedUsing = OnRep_WinningTeamID)
	int32 WinningTeamID = -1;

	FGameplayTag MatchDecidedTag;

	/* Assault */
	UFUNCTION()
	void OnRep_AssaultPoints(); 

	UPROPERTY()
	TEnumAsByte<EOccupationState> Team1OccupationState; 

	UPROPERTY(ReplicatedUsing = OnRep_Team1OccupationProgress)
	float Team1OccupationProgress = 0.f; 

	UPROPERTY(ReplicatedUsing = OnRep_Team1AssaultScore)
	int32 Team1AssaultScore = 0; 

	UPROPERTY(Replicated)
	float Team1RemainingTime = 240.f; 

	UPROPERTY()
	TEnumAsByte<EOccupationState> Team2OccupationState;

	UPROPERTY(ReplicatedUsing = OnRep_Team2OccupationProgress)
	float Team2OccupationProgress = 0.f; 

	UPROPERTY(Replicated)
	int32 Team2AssaultScore = 0;

	UPROPERTY(Replicated)
	float Team2RemainingTime = 240.f;

	/* Game Phase */
	UFUNCTION()
	void ConnectToGamePhase(); 

	UFUNCTION()
	void Countdown();

	UFUNCTION()
	void OnRep_CountdownTime(); 

	UFUNCTION()
	void OnRep_Team1OccupationProgress();

	UFUNCTION()
	void OnRep_Team1AssaultScore(); 

	UFUNCTION()
	void OnRep_Team2OccupationProgress(); 

	UFUNCTION()
	void OnRep_Team2AssaultScore();

	UFUNCTION()
	void OnRep_WinningTeamID(); 

	void GamePhaseStarted(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleFirstHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration); 

	UFUNCTION()
	void HandleFirstMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleFirstTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSwitchInningPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void PrepareNextGamePhase(); 

	FTimerHandle DelayTimerHandle; 
	FTimerHandle CountdownTimerHandle; 
	FTimerHandle GameplayEffectTimerHandle; 

	bool bFirstTeamOffenseMatchDecided = false; 
	bool bSecondTeamOffenseMatchDecided = false; 
};
