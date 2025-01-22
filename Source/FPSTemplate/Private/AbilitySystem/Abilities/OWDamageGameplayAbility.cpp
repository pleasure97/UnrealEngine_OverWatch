// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UOWDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f); 

	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel()); 
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage); 

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), 
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)); 
}
