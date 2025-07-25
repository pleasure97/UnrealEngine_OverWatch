// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "OWGamePhaseSubsystem.generated.h"

class UOWGamePhaseAbility; 

DECLARE_DELEGATE_OneParam(FOWGamePhaseDelegate, const UOWGamePhaseAbility* GamePhase);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOWGamePhaseDynamicDelegate, const UOWGamePhaseAbility*, GamePhase);
DECLARE_DELEGATE_TwoParams(FOWGamePhaseTagDelegate, const FGameplayTag& PhaseTag, const float PhaseDuration); 
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOWGamePhaseTagDynamicDelegate, const FGameplayTag&, PhaseTag, const float, PhaseDuration);

UENUM(BlueprintType)
enum class EPhaseTagMatchType : uint8
{
	ExactMatch, 
	PartialMatch 
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWGamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool IsPhaseActive(const FGameplayTag& GamePhaseTag) const;

	void OnBeginPhase(const UOWGamePhaseAbility* GamePhaseAbility, const FGameplayAbilitySpecHandle GamePhaseAbilitySpecHandle);
	void OnEndPhase(const UOWGamePhaseAbility* GamePhaseAbility, const FGameplayAbilitySpecHandle GamePhaseAbilitySpecHandle);

	void StartPhase(TSubclassOf<UOWGamePhaseAbility> GamePhaseAbility, FOWGamePhaseDelegate PhaseEndedDelegate = FOWGamePhaseDelegate());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "Start Phase", AutoCreateRefTerm = "PhaseEnded"))
	void K2_StartPhase(TSubclassOf<UOWGamePhaseAbility> Phase, const FOWGamePhaseDynamicDelegate& PhaseEnded);

	void WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FOWGamePhaseTagDelegate& WhenPhaseActive);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "When Phase Starts or Is Active", AutoCreateRefTerm = "WhenPhaseActive"))
	void K2_WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, FOWGamePhaseTagDynamicDelegate WhenPhaseActive);

	void WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FOWGamePhaseTagDelegate& WhenPhaseEnd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "When Phase Ends", AutoCreateRefTerm = "WhenPhaseEnd"))
	void K2_WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, FOWGamePhaseTagDynamicDelegate WhenPhaseEnd);

private:
	struct FOWGamePhaseEntry
	{
	public:
		FGameplayTag GamePhaseTag; 
		FOWGamePhaseDelegate PhaseEndedCallback; 
		float GamePhaseDuration; 
	};

	TMap<FGameplayAbilitySpecHandle, FOWGamePhaseEntry> ActivePhaseMap;

	struct FPhaseObserver
	{
		inline bool IsMatch(const FGameplayTag& ComparePhaseTag) const
		{
			switch (MatchType)
			{
			case EPhaseTagMatchType::ExactMatch:
				return ComparePhaseTag == PhaseTag; 
			case EPhaseTagMatchType::PartialMatch:
				return ComparePhaseTag.MatchesTag(PhaseTag); 
			}
			return false;
		}
		FGameplayTag PhaseTag;
		EPhaseTagMatchType MatchType = EPhaseTagMatchType::ExactMatch; 
		FOWGamePhaseTagDelegate PhaseCallback; 
	};

	TArray<FPhaseObserver> PhaseStartObservers;
	TArray<FPhaseObserver> PhaseEndObservers; 
};
