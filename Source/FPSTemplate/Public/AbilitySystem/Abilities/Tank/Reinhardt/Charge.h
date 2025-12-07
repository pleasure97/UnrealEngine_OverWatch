// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "Charge.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCharge : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChargeSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChargeDuration = 3.f;
};
