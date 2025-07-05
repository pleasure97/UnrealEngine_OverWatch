// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "WeaponReload.generated.h"

class UAnimMontage; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UWeaponReload : public UOWGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ReloadMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontagePlayRate = 1.f; 

	UFUNCTION(BlueprintCallable)
	void ReloadBullets(); 
};
