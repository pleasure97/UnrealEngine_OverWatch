// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "RocketHammer.generated.h"

class UAnimMontage; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API URocketHammer : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> RocketHammerMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HammerRadius = 400.f;
};
