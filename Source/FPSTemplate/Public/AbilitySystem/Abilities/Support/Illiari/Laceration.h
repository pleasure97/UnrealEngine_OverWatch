// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "Laceration.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API ULaceration : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laceration")
	float ExplosionDelayTime = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laceration")
	float RequiredDamageToExplode = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laceration")
	float ExplosionInnerRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laceration")
	float ExplosionOuterRadius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laceration")
	float ExplosionInnerRadiusDamage = 160.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laceration")
	float ExplosionOuterRadiusDamage = 120.f;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
