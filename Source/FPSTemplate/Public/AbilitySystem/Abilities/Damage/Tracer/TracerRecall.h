// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "TracerRecall.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTracerRecall : public UOWGameplayAbility
{
	GENERATED_BODY()

protected:
	/* Game Logic */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RecallTotalDuration = 0.9f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RecallIntervalDuration = 0.03f;
};
