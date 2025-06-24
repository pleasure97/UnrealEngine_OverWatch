// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "MatchPreparation.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UMatchPreparation : public UOWGamePhaseAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MatchPreparationTime = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CountdownDuration = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageImmuneGamelayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UOWGamePhaseAbility> MatchInProgress; 

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	int32 RemainingCountdown = 0; 

	FTimerHandle BeforeCountdownTimerHandle; 
	FTimerHandle CountdownTimerHandle; 

	void BroadcastCountdownMessage(); 
};
