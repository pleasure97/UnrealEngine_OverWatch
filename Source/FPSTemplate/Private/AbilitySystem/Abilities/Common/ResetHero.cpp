// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/ResetHero.h"
#include "OWGameplayTags.h"

UResetHero::UResetHero()
{
	// Instancing Policy - Instanced Per Actor
	// Net Execution Policy - Server Initiated 
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated; 

	// Add Gameplay Event Trigger Data when this Ability is CDO 
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData; 
		TriggerData.TriggerTag = FOWGameplayTags::Get().Event_ResetHero; 
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent; 
		AbilityTriggers.Add(TriggerData); 
	}
}