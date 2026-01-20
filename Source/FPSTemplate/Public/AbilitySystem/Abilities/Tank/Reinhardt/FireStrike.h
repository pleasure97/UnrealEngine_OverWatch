// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "FireStrike.generated.h"

class AnimMontage; 
class AOWProjectile; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UFireStrike : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AOWProjectile> FireStrikeClass; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<AOWProjectile> FireStrike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> FireStrikeMontage; 
};
