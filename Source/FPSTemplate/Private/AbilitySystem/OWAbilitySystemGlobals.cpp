// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemGlobals.h"
#include "OWAbilityTypes.h"

FGameplayEffectContext* UOWAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FOWGameplayEffectContext();
}
