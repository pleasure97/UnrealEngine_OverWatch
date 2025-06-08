// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "OWGamePhaseSubsystem.generated.h"

class UOWGamePhaseAbility; 

DECLARE_DYNAMIC_DELEGATE_OneParam(FOWGamePhaseDynamicDelegate, const UOWGamePhaseAbility*, GamePhase);
DECLARE_DELEGATE_OneParam(FOWGamePhaseDelegate, const UOWGamePhaseAbility* GamePhase);

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

protected:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void StartPhase(TSubclassOf<UOWGamePhaseAbility> GamePhaseAbility, const FOWGamePhaseDynamicDelegate& PhaseEndedDelegate);

private:
	struct FOWGamePhaseEntry
	{
	public:
		FGameplayTag GamePhaseTag; 
		FOWGamePhaseDelegate PhaseEndedCallback; 
	};

	TMap<FGameplayAbilitySpecHandle, FOWGamePhaseEntry> ActivePhaseMap; 
};
