// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "GameplayTagContainer.h"
#include "HeroSelectionPhase.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroSelectionPhase : public UOWGamePhaseAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HeroSelectionTime = 30.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> MatchPreparation; 
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	void ClearHeroSelectionCue(); 
	void ActivateHeroSelectionCue(const FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters);

	FGameplayTag ActiveHeroSelectionCue; 

	FTimerHandle HeroSelectionTimerHandle; 
};
