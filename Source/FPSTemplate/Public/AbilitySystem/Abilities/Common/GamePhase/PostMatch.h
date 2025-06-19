// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "PostMatch.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPostMatch : public UOWGamePhaseAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageImmuneGamelayEffectClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float NextGameInterval = 10.f; 

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
