// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AsyncTasks/WaitInteractableTargets.h"
#include "Interaction/InteractionOption.h"
#include "WaitInteractableTargets_SingleLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UWaitInteractableTargets_SingleLineTrace : public UWaitInteractableTargets
{
	GENERATED_BODY()
	
public:
	virtual void Activate() override; 

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWaitInteractableTargets_SingleLineTrace* WaitInteractableTargets_SingleLineTrace(
		UGameplayAbility* OwningAbility,
		FInteractionQuery InteractionQuery, FCollisionProfileName CollisionProfileName, 
		FGameplayAbilityTargetingLocationInfo StartLocation, 
		float InteractionScanRange = 100.f, float InteractionScanRate = 0.1f, 
		bool bShowDebug = false); 
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformTrace();

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation; 

	UPROPERTY()
	FInteractionQuery InteractionQuery; 

	FTimerHandle TimerHandle; 

	float InteractionScanRange = 100.f; 
	float InteractionScanRate = 0.1f; 

	bool bShowDebug = false; 
};
