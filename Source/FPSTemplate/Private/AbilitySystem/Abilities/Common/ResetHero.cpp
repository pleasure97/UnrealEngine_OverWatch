// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/ResetHero.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "Character/OWCharacter.h"
#include "Message/OWMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"

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

void UResetHero::SetCanBeCanceled(bool bCanBeCanceled)
{
	if (!bCanBeCanceled)
	{
		return; 
	}

	Super::SetCanBeCanceled(bCanBeCanceled); 
}

void UResetHero::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo); 

	// Get Custom Ability System Component
	UOWAbilitySystemComponent* OWAbilitySystemComponent = CastChecked<UOWAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()); 

	// Declare GameplayTag Container to be Ignored
	FGameplayTagContainer AbilityTypesToIgnore; 
	// TODO - AbilityTypesToIgnore.AddTag(FOWGameplayTags::Get()); 

	// Cancel All Abilities
	OWAbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore, this); 

	SetCanBeCanceled(false); 

	if (AOWCharacter* OWCharacter = Cast<AOWCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		OWCharacter->Reset(); 
	}

	// Broadcast Reset Message to Others 
	FHeroResetInfo HeroResetInfo; 
	HeroResetInfo.OwnerPlayerState = CurrentActorInfo->OwnerActor.Get(); 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 
	GameplayMessageSubsystem.BroadcastMessage(FOWGameplayTags::Get().Event_ResetHero, HeroResetInfo); 

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false); 
}

