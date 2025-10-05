// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"

float UOWGameplayAbility::GetSkillCost(float InLevel) const
{
    float SkillCost = 0.f; 
    if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if (Mod.Attribute == UOWAttributeSet::GetSkillGaugeAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, SkillCost); 
                break; 
            }
        }
    }

    return SkillCost; 
}

float UOWGameplayAbility::GetCooldown(float InLevel)
{
    float Cooldown = 0.f; 
    if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
    {
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown); 
    }

    return Cooldown;
}

void UOWGameplayAbility::OnHeroSet()
{
    K2_OnHeroSet();
}

void UOWGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec); 

    // Apply Stack Change Gameplay Effect when Ability is Given 
    if (ActorInfo && ActorInfo->IsNetAuthority())
    {
        ApplyStackChangeGameplayEffect(ActorInfo, Spec); 
    }
}

void UOWGameplayAbility::ApplyStackChangeGameplayEffect(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    // Early Return if Type of Gameplay Ability is Not Ability Stacking 
    if (AbilityStackingSlot == EAbilityStackingSlot::None)
    {
        return; 
    }

    // Create Runtime Memory Object (Not Asset) and Set Instant Duration Policy 
    UGameplayEffect* StackChangeGameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("StackChange"))); 
    StackChangeGameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant; 

    // Add 2 Slots to Modifiers Array (Max Stack and Current Stack)
    const int32 Index = StackChangeGameplayEffect->Modifiers.Num(); 
    StackChangeGameplayEffect->Modifiers.SetNum(Index + 2); 

    // Override Max Stack Attribute to MaxStacks Member Variable
    FGameplayModifierInfo& MaxStackInfo = StackChangeGameplayEffect->Modifiers[Index]; 
    MaxStackInfo.ModifierMagnitude = FScalableFloat(MaxStacks); 
    MaxStackInfo.ModifierOp = EGameplayModOp::Override; 

    // Override Current Stack Attribute to MaxStacks Member Varaible
    FGameplayModifierInfo& CurrentStackInfo = StackChangeGameplayEffect->Modifiers[Index + 1]; 
    CurrentStackInfo.ModifierMagnitude = FScalableFloat(MaxStacks); 
    CurrentStackInfo.ModifierOp = EGameplayModOp::Override; 

    // TODO - Enum Ability Stacking Slot or Spec's Input ID 
    switch (AbilityStackingSlot)
    {
    case EAbilityStackingSlot::FirstSkill:
    {
        MaxStackInfo.Attribute = UOWAttributeSet::GetFirstSkillMaxStacksAttribute();
        CurrentStackInfo.Attribute = UOWAttributeSet::GetFirstSkillCurrentStacksAttribute();
    }
    }

    // Cast Ability System Component from Actor Info to Custom Ability System Component
    UOWAbilitySystemComponent* OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
    // Apply Stack Change Gameplay Effect Using Custom Ability System Component 
    if (OWAbilitySystemComponent)
    {
        OWAbilitySystemComponent->ApplyGameplayEffectToSelf(StackChangeGameplayEffect, 1.f, OWAbilitySystemComponent->MakeEffectContext());
    }
}

bool UOWGameplayAbility::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool bEndAbilityImmediately)
{
    UOWAbilitySystemComponent* OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()); 
    if (OWAbilitySystemComponent)
    {
        return OWAbilitySystemComponent->BatchRPCTryActivateAbility(InAbilityHandle, bEndAbilityImmediately); 
    }
    return false;
}

void UOWGameplayAbility::ExternalEndAbility()
{
    check(CurrentActorInfo); 

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true /* bReplicateEndAbility */, false /* bWasCancelled*/); 
}
