// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
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
        InitializeAbilityStacking(ActorInfo, Spec);
    }  
}

bool UOWGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    // Override CanActivateAbility() of Parent Class if Ability Type is Not Stacking 
    if (AbilityStackingSlot == EAbilityStackingSlot::None)
    {
        return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
    }
    // Initialize Boolean Value Indicating Whether Stack Attribute Exists
    bool bHasStackAttribute = false;
    // Get Current Stack Attribute Value from Avatar Actor Using Ability System Blueprint Library 
    float CurrentStackAttributeValue = UAbilitySystemBlueprintLibrary::GetFloatAttribute(
        ActorInfo->AvatarActor.Get(), 
        CurrentStackAttribute, 
        bHasStackAttribute); 

    // Return Whether the Stack is Greater than 0 to Activate the Ability
    return (bHasStackAttribute && CurrentStackAttributeValue > 0.f);
}

void UOWGameplayAbility::InitializeAbilityStacking(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    // Early Return if Type of Gameplay Ability is Not Ability Stacking 
    if (AbilityStackingSlot == EAbilityStackingSlot::None)
    {
        return; 
    }

    // Check if Max or Current Stack Attribute is Valid 
    if (!MaxStackAttribute.IsValid() || !CurrentStackAttribute.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Max or Current Stack Attribute is Not Valid in UOWGameplayAbility::InitializeAbilityStacking()")); 
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

    // Set Attributes of Max and Current Stack Info
    MaxStackInfo.Attribute = MaxStackAttribute;
    CurrentStackInfo.Attribute = CurrentStackAttribute; 

    // Allocate Gameplay Effect Context and Create it on Heap Memory 
    FGameplayEffectContext* StackGameplayEffectContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext(); 
    
    // Create Gameplay Effect Spec with Runtime Gameplay Effect Context
    const FGameplayEffectSpecHandle StackGameplayEffectSpecHandle =
        FGameplayEffectSpecHandle(
            new FGameplayEffectSpec(StackChangeGameplayEffect, FGameplayEffectContextHandle(StackGameplayEffectContextHandle), 1.f)); 

    // Apply Stack Gameplay Effect Spec to Owner 
    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        ASC->ApplyGameplayEffectSpecToSelf(*StackGameplayEffectSpecHandle.Data.Get()); 
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
