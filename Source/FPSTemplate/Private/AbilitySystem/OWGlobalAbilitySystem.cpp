// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWGlobalAbilitySystem.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"


void FOWGlobalAbilityList::AddToASC(TSubclassOf<UGameplayAbility> Ability, UOWAbilitySystemComponent* ASC)
{
	if (FGameplayAbilitySpecHandle* GameplayAbilitySpecHandle = Handles.Find(ASC))
	{
		RemoveFromASC(ASC); 
	}

	UGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UGameplayAbility>(); 
	FGameplayAbilitySpec AbilitySpec(AbilityCDO); 
	const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec); 
	Handles.Add(ASC, AbilitySpecHandle); 
}

void FOWGlobalAbilityList::RemoveFromASC(UOWAbilitySystemComponent* ASC)
{
	if (FGameplayAbilitySpecHandle* GameplayAbilitySpecHandle = Handles.Find(ASC))
	{
		ASC->ClearAbility(*GameplayAbilitySpecHandle); 
		Handles.Remove(ASC); 
	}
}

void FOWGlobalAbilityList::RemoveFromAll()
{
	for (TPair<TObjectPtr<UOWAbilitySystemComponent>, FGameplayAbilitySpecHandle>& Handle : Handles)
	{
		if (Handle.Key != nullptr)
		{
			Handle.Key->ClearAbility(Handle.Value);
		}
	}
	Handles.Empty(); 
}

void FOWGlobalEffectList::AddToASC(TSubclassOf<UGameplayEffect> Effect, UOWAbilitySystemComponent* ASC)
{
	if (FActiveGameplayEffectHandle* EffectHandle = Handles.Find(ASC))
	{
		RemoveFromASC(ASC); 
	}

	const UGameplayEffect* GameplayEffectCDO = Effect->GetDefaultObject<UGameplayEffect>();
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffectCDO, 1, ASC->MakeEffectContext());
	Handles.Add(ASC, ActiveGameplayEffectHandle);
}

void FOWGlobalEffectList::RemoveFromASC(UOWAbilitySystemComponent* ASC)
{
	if (FActiveGameplayEffectHandle* ActiveGameplayEffectHandle = Handles.Find(ASC))
	{
		ASC->RemoveActiveGameplayEffect(*ActiveGameplayEffectHandle);
		Handles.Remove(ASC); 
	}
}

void FOWGlobalEffectList::RemoveFromAll()
{
	for (TPair<TObjectPtr<UOWAbilitySystemComponent>, FActiveGameplayEffectHandle>& Handle : Handles)
	{
		if (Handle.Key != nullptr)
		{
			Handle.Key->RemoveActiveGameplayEffect(Handle.Value); 
		}
	}
	Handles.Empty(); 
}

UOWGlobalAbilitySystem::UOWGlobalAbilitySystem()
{
}

void UOWGlobalAbilitySystem::ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect)
{
	if ((Effect.Get() != nullptr) && (!AppliedEffects.Contains(Effect)))
	{
		FOWGlobalEffectList& GlobalEffectEntry = AppliedEffects.Add(Effect); 
		for (UOWAbilitySystemComponent* RegisteredASC : RegisteredASCs)
		{
			GlobalEffectEntry.AddToASC(Effect, RegisteredASC); 
		}
	}
}

void UOWGlobalAbilitySystem::RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect)
{
	if ((Effect.Get() != nullptr) && (AppliedEffects.Contains(Effect)))
	{
		FOWGlobalEffectList& GlobalEffectEntry = AppliedEffects[Effect]; 
		GlobalEffectEntry.RemoveFromAll();
		AppliedEffects.Remove(Effect); 
	}
}

void UOWGlobalAbilitySystem::RegisterASC(UOWAbilitySystemComponent* ASC)
{
	check(ASC); 

	for (TPair<TSubclassOf<UGameplayAbility>, FOWGlobalAbilityList>& AppliedAbility : AppliedAbilities)
	{
		AppliedAbility.Value.AddToASC(AppliedAbility.Key, ASC); 
	}
	for (TPair<TSubclassOf<UGameplayEffect>, FOWGlobalEffectList>& AppliedEffect : AppliedEffects)
	{
		AppliedEffect.Value.AddToASC(AppliedEffect.Key, ASC); 
	}

	RegisteredASCs.AddUnique(ASC);
}

void UOWGlobalAbilitySystem::UnregisterASC(UOWAbilitySystemComponent* ASC)
{
	check(ASC); 

	for (TPair<TSubclassOf<UGameplayAbility>, FOWGlobalAbilityList>& AppliedAbility : AppliedAbilities)
	{
		AppliedAbility.Value.RemoveFromASC(ASC);
	}
	for (TPair<TSubclassOf<UGameplayEffect>, FOWGlobalEffectList>& AppliedEffect : AppliedEffects)
	{
		AppliedEffect.Value.RemoveFromASC(ASC); 
	}

	RegisteredASCs.Remove(ASC); 
}