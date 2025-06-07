// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/AutoReload.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "OWGameplayTags.h"

bool UAutoReload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// Check Num Max Bullets is Greater than 0 
	// If not, Auto Reload is not Activated
	float NumMaxBullets = ActorInfo->AbilitySystemComponent->GetNumericAttribute(UOWAttributeSet::GetNumMaxBulletsAttribute());
	
	return (NumMaxBullets > 0.f) && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags); 
}

void UAutoReload::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	PeriodicCheck(); 
}

void UAutoReload::PeriodicCheck()
{
	WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, PollInterval);
	WaitDelayTask->OnFinish.AddDynamic(this, &UAutoReload::Reload);
	WaitDelayTask->ReadyForActivation();
}

void UAutoReload::Reload()
{
	check(CurrentActorInfo); 

	float NumCurrentBullets = CurrentActorInfo->AbilitySystemComponent->GetNumericAttribute(UOWAttributeSet::GetNumCurrentBulletsAttribute());

	if (NumCurrentBullets > 0.f)
	{
		return;
	}

	if (!CurrentActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FOWGameplayTags::Get().Event_Movement_Reload))
	{
		FGameplayEventData Payload;
		SendGameplayEvent(FOWGameplayTags::Get().InputTag_Reload, Payload);
	}

	PeriodicCheck(); 
}

