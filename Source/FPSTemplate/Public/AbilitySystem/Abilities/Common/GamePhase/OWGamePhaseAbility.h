// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OWGamePhaseAbility.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWGamePhaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UOWGamePhaseAbility(); 

	const FGameplayTag& GetGamePhaseTag() const { return GamePhaseTag; }
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override; 

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Phase")
	FGameplayTag GamePhaseTag; 
};
