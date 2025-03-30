// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/HealingPylon.h"

void UHealingPylon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	CommitAbilityCost(Handle, ActorInfo, ActivationInfo); 

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true); 

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false); 
}
