// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "OWGamePhaseSubsystem.generated.h"

class UOWGamePhaseAbility; 

DECLARE_DELEGATE_OneParam(FOWGamePhaseDelegate, const UOWGamePhaseAbility* GamePhase);
DECLARE_DELEGATE_OneParam(FOWGamePhaseTagDelegate, const FGameplayTag& PhaseTag); 

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

	void WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FOWGamePhaseTagDelegate& WhenPhaseActive);
	void WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FOWGamePhaseTagDelegate& WhenPhaseEnd);

private:
	struct FOWGamePhaseEntry
	{
	public:
		FGameplayTag GamePhaseTag; 
		FOWGamePhaseDelegate PhaseEndedCallback; 
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
