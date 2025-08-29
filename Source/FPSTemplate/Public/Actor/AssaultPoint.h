// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Message/OWMessageTypes.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "GameplayTagContainer.h"
#include "AssaultPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumAttackersChanged, int32, NewNumAttackers); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumDefendersChanged, int32, NewNumDefenders); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOccupationProgressChanged, float, NewOccupationProgress); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOccupationStateChanged, EOccupationState, NewOccupationState); 

class UBoxComponent; 
class APawn; 
class UMatchScoringComponent; 
class UAbilitySystemComponent; 

UCLASS()
class FPSTEMPLATE_API AAssaultPoint : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AAssaultPoint();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseCaptureRate = 1 / 120.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TwoPeopleMultiplier = 1.5f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ThreePeopleMultiplier = 2.f; 

	UPROPERTY(BlueprintAssignable)
	FOnNumAttackersChanged OnNumAttackersChanged; 

	UPROPERTY(BlueprintAssignable)
	FOnNumDefendersChanged OnNumDefendersChanged;

	UPROPERTY(BlueprintAssignable)
	FOnOccupationProgressChanged OnOccupationProgressChanged;

	UPROPERTY(BlueprintAssignable)
	FOnOccupationStateChanged OnOccupationStateChanged;

	UFUNCTION(Server, Reliable)
	void Server_SendAssaultScore(const FGameplayTag& CurrentGamePhaseTag, const float DecidedOccupationProgress); 

	bool IsAssaultPointActive() const { return bAssaultPointActive; }
	int32 GetNumAttackers() const { return NumAttackers; }
	int32 GetNumDefenders() const { return NumDefenders; }
	float GetOccupationProgress() const { return OccupationProgress; }
	EOccupationState GetOccupationState() const { return OccupationState; }
	void SetAssaultPointID(int32 InAssaultPointID) { AssaultPointID = InAssaultPointID; }

protected:
	virtual void BeginPlay() override; 

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override; 

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(ReplicatedUsing=OnRep_NumAttackers)
	int32 NumAttackers = 0; 

	UPROPERTY(ReplicatedUsing=OnRep_NumDefenders)
	int32 NumDefenders = 0; 

	UPROPERTY(ReplicatedUsing=OnRep_OccupationProgress)
	float OccupationProgress = 0.f; 

	UPROPERTY(ReplicatedUsing = OnRep_OccupationState)
	TEnumAsByte<EOccupationState> OccupationState = EOccupationState::None;

	float LastBroadcastedOccupationProgress = 0.f; 

	UPROPERTY(Replicated)
	int32 AssaultPointID = 2; 

	UPROPERTY(Replicated)
	TArray<AActor*> OverlappingPawns; 

	UPROPERTY()
	TEnumAsByte<EOccupationState> LastOccupationState = EOccupationState::None;

	UPROPERTY()
	TObjectPtr<UMatchScoringComponent> MatchScoringComponent; 

	FOWGamePhaseTagDelegate FirstTeamOffensePhaseDelegate; 
	FOWGamePhaseTagDelegate SwitchInningPhaseDelegate; 
	FOWGamePhaseTagDelegate SecondTeamOffensePhaseDelegate; 

	FTimerHandle PhaseEndTimerHandle;

	bool bAssaultPointActive = true; 
	bool bMatchDecided = false; 

	UFUNCTION()
	void HandleFirstTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSwitchInningPhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void HandleSecondTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration);

	void MeasureWhenPhaseEnds(const FGameplayTag& PhaseTag, const float PhaseDuration);

	UFUNCTION()
	void SendAssaultScoreWhenPhaseEnds(); 

	void UpdateAssaultPoint(int32 NewNumAttackers, int32 NewNumDefenders); 

	UFUNCTION()
	void OnRep_NumAttackers(); 

	UFUNCTION()
	void OnRep_NumDefenders();

	UFUNCTION()
	void OnRep_OccupationProgress(); 

	UFUNCTION()
	void OnRep_OccupationState(); 
};
