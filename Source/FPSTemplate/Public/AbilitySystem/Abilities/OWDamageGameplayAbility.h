// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "OWDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWDamageGameplayAbility : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	void CauseDamage(AActor* TargetActor); 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;
};
