// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"

void UOWAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UOWAbilitySystemComponent::ClientEffectApplied);

    const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
}

void UOWAbilitySystemComponent::AddHeroAbilities(const TArray<TSubclassOf<UGameplayAbility>>& DefaultAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1 /*int32 InLevel*/);
        if (const UOWGameplayAbility* OWAbility = Cast<UOWGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.DynamicAbilityTags.AddTag(OWAbility->DefaultInputTag); 
            AbilitySpec.DynamicAbilityTags.AddTag(FOWGameplayTags::Get().Abilities_Status_Equipped); 
            GiveAbility(AbilitySpec); 
        }
    }
    bDefaultAbilitiesGiven = true; 
    AbilitiesGivenDelegate.Broadcast(); 
}


void UOWAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer GameplayTagContainer; 
    EffectSpec.GetAllAssetTags(GameplayTagContainer); 

    EffectAssetTags.Broadcast(GameplayTagContainer); 
}

