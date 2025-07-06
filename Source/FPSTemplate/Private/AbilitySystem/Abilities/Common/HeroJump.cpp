// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/HeroJump.h"
#include "Character/OWCharacter.h"

UHeroJump::UHeroJump()
{
	// Instancing Policy - Instanced Per Actor, Net Execution Policy - Local Predicted
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; 
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted; 
}

bool UHeroJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// Check if Actor Info and Avatar Actor is Valid 
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false; 
	}

	// Cast Avatar Actor to Custom Character and Check if Character Can Jump 
	const AOWCharacter* OWCharacater = Cast<AOWCharacter>(ActorInfo->AvatarActor.Get()); 
	if (!OWCharacater || !OWCharacater->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true; 
}

void UHeroJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	HeroJumpStop(); 

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled); 
}

void UHeroJump::HeroJumpStart()
{
	if (AOWCharacter* OWCharacater = Cast<AOWCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		if (OWCharacater->IsLocallyControlled() && !OWCharacater->bPressedJump)
		{
			OWCharacater->UnCrouch(); 
			OWCharacater->Jump(); 
		}
	}
}

void UHeroJump::HeroJumpStop()
{
	if (AOWCharacter* OWCharacter = Cast<AOWCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		if (OWCharacter->IsLocallyControlled() && OWCharacter->bPressedJump)
		{
			OWCharacter->StopJumping();
		}
	}
}






