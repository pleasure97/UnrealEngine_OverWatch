// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WeaponReload.generated.h"

class UAnimMontage; 
class UAbilityTask_PlayMontageAndWait;
class UAbilityAsync_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UWeaponReload : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ReloadMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontagePlayRate = 1.f; 

protected:
	bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageAndWaitTask; 

	UPROPERTY()
	TObjectPtr<UAbilityAsync_WaitGameplayEvent> WaitGameplayEventTask; 

	UFUNCTION()
	void OnCompleted(); 
	
	UFUNCTION()
	void OnInterrupted(); 

	UFUNCTION()
	void OnCancelled(); 

	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload); 
};
