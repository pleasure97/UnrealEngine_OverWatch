// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "HealingPylon.generated.h"

class AHealingSunStone; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealingPylon : public UOWGameplayAbility
{
	GENERATED_BODY()

public:
	/* Healing Sun Stone */

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf <AHealingSunStone> HealingSunStoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector PylonRelativeLocation = FVector(56.f, 2.f, 56.f); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PylonThrowingSpeed = 800.f; 
};
