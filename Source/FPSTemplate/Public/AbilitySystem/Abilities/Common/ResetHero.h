// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ResetHero.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UResetHero : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UResetHero(); 

protected:
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override; 

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override; 
};
