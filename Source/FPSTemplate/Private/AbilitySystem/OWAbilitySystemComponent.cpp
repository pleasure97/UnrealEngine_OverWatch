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

void UOWAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;

    FScopedAbilityListLock ActiveScopeLock(*this); 
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec); 
            if (AbilitySpec.IsActive())
            {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey()); 
            }
        }
    }
}

void UOWAbilitySystemComponent::AbilityInuptTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return; 

    FScopedAbilityListLock ActiveScopeLock(*this); 
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec); 
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle); 
            }
        }
    }
}

void UOWAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return; 

    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
        {
            AbilitySpecInputReleased(AbilitySpec); 
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey()); 
        }
    }
}



void UOWAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer GameplayTagContainer; 
    EffectSpec.GetAllAssetTags(GameplayTagContainer); 

    EffectAssetTags.Broadcast(GameplayTagContainer); 
}

