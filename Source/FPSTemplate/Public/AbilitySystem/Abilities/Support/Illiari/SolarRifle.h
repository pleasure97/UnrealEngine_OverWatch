// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "SolarRifle.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API USolarRifle : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> RechargeGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DelayRechargeGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBetweenShots = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RechargeWaitingTime = 0.25f;
};
