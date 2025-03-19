// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "CaptiveSun.generated.h"

class UTimelineComponent; 
class ACaptiveSunProjectile; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCaptiveSun : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override; 
	
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter; 

	/* Soaring */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Soaring")
	TObjectPtr<UCurveFloat> VelocityCurve; 

	UPROPERTY(VisibleAnywhere, Category="Soaring")
	TObjectPtr<UTimelineComponent> SoaringVelocity; 

	UFUNCTION()
	void UpdateSoaringVelocity(float Velocity); 

	UFUNCTION()
	void FinishSoaring(); 
	
	/* Spawn Projectile*/
	UFUNCTION(BlueprintCallable)
	void SpawnCaptiveSun(); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACaptiveSunProjectile> CaptiveSunProjectileClass;

	UPROPERTY()
	TObjectPtr<ACaptiveSunProjectile> CaptiveSunProjectile;

	/* Shoot */
	UFUNCTION()
	void Shoot(float TimeWaited);
};
