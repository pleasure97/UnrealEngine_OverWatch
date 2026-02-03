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
	TSubclassOf<UGameplayEffect> RechargeGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DelayRechargeGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenHeal = 0.1f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealPerUnitTime = 11.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GeneralRechargeWaitingTime = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RechargeWaitingTimeWhenCompletelyConsumed = 1.f;
};