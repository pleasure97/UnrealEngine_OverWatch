// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/HealingRay.h"

bool UHealingRay::RegenerateHealGaugeByTick()
{
    if (HealGauge >= MaxHealGauge)
    {
        // bFullCharged 
        return true; 
    }
    else
    {
        HealGauge += HealRegeneration; 
        return false; 
    }
}
