// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "HealingRay.generated.h"

class UAnimMontage; 
class UNiagaraSystem; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealingRay : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	void PrepareToEndAbility(); 

	/* Heal Logic */
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload); 

	UFUNCTION()
	void OnInputRelease(float TimeHeld); 

	UFUNCTION(BlueprintCallable)
	void SpawnHealBeam();

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter; 

	UPROPERTY()
	TObjectPtr<AActor> HealedTarget; 

	UPROPERTY()
	FGameplayCueParameters TargetCueParameters; 
	
	bool bTargetImplementsInterface = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealGauge = 445.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealGauge = 445.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealElapsedSeconds = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount = 115.f;

	UFUNCTION(BlueprintCallable)
	void HealAndCost();

	void ClearTimerAndEndAbility();

	void ApplyHeal(AActor* TargetActor);

	FTimerHandle HealGaugeTimerHandle; 

	/* Anim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> HealingRayMontage;

	// TODO - Move to Gameplay Cue Notify Actor 
	/* Heal Niagara Effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> HealLaser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> HealLaserImpact;

	/* Heal Regeneration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealRegeneration = 42.5f;

	UFUNCTION(BlueprintCallable)
	void RegenerateHealGauage();

	float LastHealTime = 0.f;

	FTimerHandle HealRegenTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimerRate = 0.1f;
};