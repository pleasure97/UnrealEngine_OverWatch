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

void UOWAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this); 

    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to execute deleagte in %hs"), __FUNCTION__); 
        }
    }
}

FGameplayTag UOWAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for (FGameplayTag AbilityTag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
            {
                return AbilityTag; 
            }
        }
    }
    return FGameplayTag(); 
}

FGameplayTag UOWAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag InputTag : AbilitySpec.DynamicAbilityTags)
    {
        if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
        {
            return InputTag; 
        }
    }
    return FGameplayTag(); 
}

FGameplayTag UOWAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
    {
        if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
        {
            return StatusTag; 
        }
    }
    return FGameplayTag(); 
}

FGameplayTag UOWAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
    if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
    {
        return GetStatusTagFromSpec(*Spec); 
    }
    return FGameplayTag(); 
}

FGameplayAbilitySpec* UOWAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopeLock(*this); 
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (Tag.MatchesTag(AbilityTag))
            {
                return &AbilitySpec; 
            }
        }
    }
    return nullptr; 
}

void UOWAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer GameplayTagContainer; 
    EffectSpec.GetAllAssetTags(GameplayTagContainer); 

    EffectAssetTags.Broadcast(GameplayTagContainer); 
}

void UOWAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities(); 

    if (!bDefaultAbilitiesGiven)
    {
        bDefaultAbilitiesGiven = true; 
        AbilitiesGivenDelegate.Broadcast(); 
    }
}

void UOWAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
    AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel); 
}

