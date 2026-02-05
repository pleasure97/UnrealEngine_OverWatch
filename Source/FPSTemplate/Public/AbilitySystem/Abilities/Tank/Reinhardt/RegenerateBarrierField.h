// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "RegenerateBarrierField.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API URegenerateBarrierField : public UOWGameplayAbility
{
	GENERATED_BODY()
	
protected:
	/* Barrier Health Regeneration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> RechargeGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DelayRechargeGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RechargeWaitingTime = 2.f;
};
