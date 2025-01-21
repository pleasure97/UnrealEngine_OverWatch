// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystem/OWAttributeSet.h"

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
