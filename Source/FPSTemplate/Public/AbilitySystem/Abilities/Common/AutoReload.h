// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AutoReload.generated.h"

class UAbilityTask_WaitDelay; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAutoReload : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PollInterval = 0.25f; 
	
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
	TObjectPtr<UAbilityTask_WaitDelay> WaitDelayTask;

	void PeriodicCheck();

	UFUNCTION()
	void Reload(); 
};
