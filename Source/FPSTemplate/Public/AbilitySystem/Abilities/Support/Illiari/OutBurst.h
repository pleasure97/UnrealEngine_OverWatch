// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "OutBurst.generated.h"

class UAnimMontage; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOutBurst : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> FirstPersonAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> ThirdPersonAnimMontage; 
};
