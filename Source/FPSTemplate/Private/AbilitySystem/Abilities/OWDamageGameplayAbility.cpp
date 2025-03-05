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

FDamageEffectParams UOWDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(
	AActor* TargetActor, 
	FVector InRadialDamageOrigin,
	bool bOverrideKnockbackForce, 
	FVector KnockbackDirectionOverride,
	bool bOverrideDeathImpulse,
	FVector DeathImpulseDirectionOverride, 
	bool bOverridePitch, 
	float PitchOverride) const
{
	FDamageEffectParams Params; 

	Params.WorldContextObject = GetAvatarActorFromActorInfo(); 
	Params.DamageGameplayEffectClass = DamageEffectClass; 
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo(); 
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); 
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel()); 
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance; 
	Params.DebuffDamage = DebuffDamage; 
	Params.DebuffDuration = DebuffDuration; 
	Params.DebuffFrequency = DebuffFrequency; 
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude; 
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude; 
	
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation(); 
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride; 
		}
		const FVector ToTarget = Rotation.Vector(); 
		if (!bOverrideKnockbackForce)
		{
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude; 
		}
		if (!bOverrideDeathImpulse)
		{
			Params.DeathImpulse = ToTarget * DeathImpulseMagnitude; 
		}
	}

	if (bOverrideKnockbackForce)
	{
		KnockbackDirectionOverride.Normalize(); 
		Params.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude; 
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation(); 
			KnockbackRotation.Pitch = PitchOverride; 
			Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude; 
		}
	}

	if (bOverrideDeathImpulse)
	{
		DeathImpulseDirectionOverride.Normalize(); 
		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation(); 
			DeathImpulseRotation.Pitch = PitchOverride; 
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude; 
		}
	}

	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = InRadialDamageOrigin; 
		Params.RadialDamageRadius = RadialDamageRadius; 
	}

	return Params; 
}

float UOWDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel()); 
}
