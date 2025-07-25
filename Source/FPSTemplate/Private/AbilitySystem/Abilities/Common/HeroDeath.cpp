// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/HeroDeath.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"

UHeroDeath::UHeroDeath()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; 
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated; 

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData AbilityTriggerData; 
		AbilityTriggerData.TriggerTag = FOWGameplayTags::Get().Event_Death; 
		AbilityTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent; 
		AbilityTriggers.Add(AbilityTriggerData); 
	}
}

void UHeroDeath::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Get custom ability system component
	UOWAbilitySystemComponent* OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()); 

	// Initialize gameplaytag container which ignores Death Activation. 
	FGameplayTagContainer AbilityTypesToIgnore; 
	AbilityTypesToIgnore.AddTag(FOWGameplayTags::Get().Abilities_IgnoreDeath); 

	// Cancel abilities except those which has IgnoreDeath GameplayTag. 
	OWAbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore, this); 

	// Make HeroDeath gameplay ability uncancellable 
	SetCanBeCanceled(false); 

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}