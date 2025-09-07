// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "HealingRay.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealingRay : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenHeal = 0.1f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealGauge = 34.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealGauge = 34.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 2000.f;

	/* Heal Regeneration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealRegeneration = 0.8f;

	UFUNCTION(BlueprintCallable)
	bool RegenerateHealGaugeByTick(); 
};