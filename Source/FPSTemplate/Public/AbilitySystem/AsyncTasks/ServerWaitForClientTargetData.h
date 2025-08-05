// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "ServerWaitForClientTargetData.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UServerWaitForClientTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UServerWaitForClientTargetData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataDelegate ValidData;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParams = "Instigator"), Category = "AbilityTasks")
	static UServerWaitForClientTargetData* ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool bInTriggerOnce);

	virtual void Activate() override; 

	UFUNCTION()
	void OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag ActivationTag); 

protected:
	virtual void OnDestroy(bool bAbilityEnded) override; 

	bool bTriggerOnce; 
};
