// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/WeaponReload.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "OWGameplayTags.h"


bool UWeaponReload::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// Check Num Max Bullets is Greater than 0 
	// Check Num Current Bullets is not equal to Num Max Bullets
	float NumMaxBullets = ActorInfo->AbilitySystemComponent->GetNumericAttribute(UOWAttributeSet::GetNumMaxBulletsAttribute());
	float NumCurrentBullets = ActorInfo->AbilitySystemComponent->GetNumericAttribute(UOWAttributeSet::GetNumCurrentBulletsAttribute());

	bool bFullBullets = FMath::IsNearlyEqual(NumMaxBullets, NumCurrentBullets); 

	return (NumMaxBullets > 0.f) && !bFullBullets && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWeaponReload::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), ReloadMontage, MontagePlayRate);
	if (PlayMontageAndWaitTask)
	{
		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UWeaponReload::OnCompleted);
		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UWeaponReload::OnInterrupted); 
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UWeaponReload::OnCancelled);
		PlayMontageAndWaitTask->ReadyForActivation();

		WaitGameplayEventTask = UAbilityAsync_WaitGameplayEvent::WaitGameplayEventToActor(
			GetAvatarActorFromActorInfo(), FOWGameplayTags::Get().Event_ReloadDone, true, true);
		if (WaitGameplayEventTask)
		{
			WaitGameplayEventTask->EventReceived.AddDynamic(this, &UWeaponReload::OnEventReceived);
		}
	}
}

void UWeaponReload::OnCompleted()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = false;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWeaponReload::OnInterrupted()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWeaponReload::OnCancelled()
{
	check(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWeaponReload::OnEventReceived(FGameplayEventData Payload)
{
	check(CurrentActorInfo);

	if (HasAuthority(&CurrentActivationInfo))
	{
		UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get(); 
		if (!AbilitySystemComponent)
		{
			return;
		}

		const UOWAttributeSet* OWAttributeSet = AbilitySystemComponent->GetSet<UOWAttributeSet>(); 
		if (!OWAttributeSet)
		{
			return;
		}

		AbilitySystemComponent->SetNumericAttributeBase(
			UOWAttributeSet::GetNumCurrentBulletsAttribute(),
			OWAttributeSet->GetNumMaxBullets()); 

		bool bReplicateEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}



