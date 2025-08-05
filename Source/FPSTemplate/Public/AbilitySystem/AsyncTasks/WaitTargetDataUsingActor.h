// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GameplayTagcontainer.h"
#include "WaitTargetDataUsingActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataUsingActorDelegate, const FGameplayAbilityTargetDataHandle&, Data); 

/**
 * Waits for Target Data from an Already Spawned Target Actor and Does NOT Destroy it when it Receives Data. 
 * Lightweight Version of WaitTargetData Ability Task 
 */
UCLASS()
class FPSTEMPLATE_API UWaitTargetDataUsingActor : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UWaitTargetDataUsingActor(const FObjectInitializer& ObjectInitializer); 

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate ValidData; 

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate Canceled; 

	// Use Specified Spawned Target Actor and Waits for it to Return Valid Data or to be Canceled.
	// @params bCreateKeyIfNotValidForMorePredicting - Create a New Scoped Prediction Key if the Current Scoped Prediction Key is Not Valid for More Predicting 
	//												 - If False, it will Always Create a New Scoped Prediction Key
	//												 - If True, we want to use a Potentially Existing Valid Scoped Prediction Key like the Ability's Activation Key in a Batched Ability
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParams = "Instigator"), Category = "AbilityTasks")
	static UWaitTargetDataUsingActor* WaitTargetDataWithReusableActor(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		TEnumAsByte<EGameplayTargetingConfirmation::Type> InConfirmationType,
		AGameplayAbilityTargetActor* InTargetActor,
		bool bInCreateKeyIfNotValidForMorePredicting = false); 
	
	virtual void Activate() override; 
	
	UFUNCTION()
	virtual void OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag ActivationTag); 

	UFUNCTION()
	virtual void OnTargetDataReplicatedCanceled(); 

	UFUNCTION()
	virtual void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle); 

	UFUNCTION()
	virtual void OnTargetDataCanceled(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle); 

	// Called when the Ability is asked to Confirm from an Outside Node. 
	virtual void ExternalConfirm(bool bEndTask) override; 

	// Called when the Ability is asked to Cancel from an Outside Node. 
	virtual void ExternalCancel() override; 
	
protected:
	UPROPERTY()
	TObjectPtr<AGameplayAbilityTargetActor> TargetActor; 

	bool bCreateKeyIfNotValidForMorePredicting; 

	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType; 

	virtual void OnDestroy(bool bAbilityEnded) override;

	virtual void InitializeTargetActor() const; 
	virtual void FinalizeTargetActor() const; 

	virtual void RegisterTargetDataCallbacks(); 

	virtual bool ShouldReplicateDataToServer() const; 
};
