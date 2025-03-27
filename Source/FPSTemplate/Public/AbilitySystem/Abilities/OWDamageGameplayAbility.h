// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "OWAbilityTypes.h"
#include "OWDamageGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FDebuffInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debuff")
	FGameplayTag DebuffTag; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff")
	float DebuffChance; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff")
	float DebuffDamage; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff")
	float DebuffDuration; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff")
	float DebuffFrequency; 
};
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWDamageGameplayAbility : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor); 

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FGameplayTag InDebuffTag = FGameplayTag(), 
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackForce = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const; 

	UFUNCTION(BlueprintCallable)
	float GetDamageAtLevel() const; 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TArray<FDebuffInfo> DebuffInfos; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageRadius = 0;
};
