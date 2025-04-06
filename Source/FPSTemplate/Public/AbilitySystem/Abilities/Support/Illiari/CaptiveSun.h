// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "CaptiveSun.generated.h"

class UTimelineComponent; 
class ACaptiveSunProjectile; 
class UUltimateDuration; 

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
	void UpdateSoaringVelocity(float Output); 

	UFUNCTION()
	void FinishSoaring(); 
	
	/* Spawn Projectile*/
	UFUNCTION(BlueprintCallable)
	void SpawnCaptiveSun(); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACaptiveSunProjectile> CaptiveSunProjectileClass;

	UPROPERTY()
	TObjectPtr<ACaptiveSunProjectile> CaptiveSunProjectile;

	/* Ultimate Duration */
	UPROPERTY()
	float Duration = 5.2f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUltimateDuration> UltimateDurationClass; 

	UPROPERTY()
	TObjectPtr<UUltimateDuration> UltimateDuration; 

	FTimerHandle DurationTickTimerHandle; 

	FTimerHandle DurationEndTimerHandle; 

	UFUNCTION()
	void UpdateDurationUI();

	UFUNCTION()
	void EndDurationUI(); 

	/* Shoot */
	UFUNCTION()
	void Shoot(float TimeWaited);
};
