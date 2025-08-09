// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitInterval.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitIntervalDelegate); 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAbilityTask_WaitInterval : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FWaitIntervalDelegate OnWaitInterval; 

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_WaitInterval* WaitInterval(UGameplayAbility* OwningAbility, float IntervalSeconds, float DurationSeconds = -1.f); 

	virtual void Activate() override; 
	virtual void OnDestroy(bool bInOwnerFinished) override; 

protected:
	void HandleWaitInterval(); 

	FTimerHandle WaitIntervalTimerHandle; 

	float IntervalSeconds; 
	float RemainingTime; 
};
