// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "PulseBomb.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPulseBomb : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PulseBombThrowingSpeed = 1500.f; 
};
