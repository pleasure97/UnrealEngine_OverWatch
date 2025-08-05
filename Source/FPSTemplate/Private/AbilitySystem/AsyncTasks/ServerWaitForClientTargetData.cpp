// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/ServerWaitForClientTargetData.h"
#include "AbilitySystemComponent.h"

UServerWaitForClientTargetData::UServerWaitForClientTargetData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UServerWaitForClientTargetData* UServerWaitForClientTargetData::ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool bInTriggerOnce)
{
	UServerWaitForClientTargetData* MyObj = NewAbilityTask<UServerWaitForClientTargetData>(OwningAbility, TaskInstanceName); 
	MyObj->bTriggerOnce = bInTriggerOnce; 
	return MyObj; 
}

void UServerWaitForClientTargetData::Activate()
{
	if (!Ability || !Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		return;
	}

	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = GetAbilitySpecHandle(); 
	FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); 
	AbilitySystemComponent->AbilityTargetDataSetDelegate(GameplayAbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UServerWaitForClientTargetData::OnTargetDataReplicated); 
}

void UServerWaitForClientTargetData::OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag ActivationTag)
{
	FGameplayAbilityTargetDataHandle MutableGameplayAbilityTargetDataHandle = GameplayAbilityTargetDataHandle; 
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); 

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(MutableGameplayAbilityTargetDataHandle); 
	}

	if (bTriggerOnce)
	{
		EndTask(); 
	}
}

void UServerWaitForClientTargetData::OnDestroy(bool bAbilityEnded)
{
	if (AbilitySystemComponent.Get())
	{
		FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = GetAbilitySpecHandle(); 
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); 
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GameplayAbilitySpecHandle, ActivationPredictionKey).RemoveAll(this); 
	}

	Super::OnDestroy(bAbilityEnded); 
}
