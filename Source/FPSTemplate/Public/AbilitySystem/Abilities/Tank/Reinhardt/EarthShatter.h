// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "EarthShatter.generated.h"

class AEarthShatterShockwave;
class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UEarthShatter : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistance = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExpansionSpeed = 4000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArcAngle = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VerticalHeight = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShockwaveDirectHitDistance = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShockwaveDirectHitDamage = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShockwaveBaseDamage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEarthShatterShockwave> EarthShatterShockwaveClass;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnEarthShatterFinished(FGameplayEventData Payload);

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;
};
