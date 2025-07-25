// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "HeroDeath.generated.h"

class UDeathOverlay; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroDeath : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroDeath(); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeathEffectDuration = 5.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RespawnWaitingDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDeathOverlay> DeathOverlayWidgetClass; 

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override; 
};
