// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "SolarRifle.generated.h"

class UNiagaraSystem; 
class AOWProjectile; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API USolarRifle : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> MuzzleFlash;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOWProjectile> SolarProjectileClass;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 40000.f;
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void HitScan(const FTransform& SocketTransform);
};
