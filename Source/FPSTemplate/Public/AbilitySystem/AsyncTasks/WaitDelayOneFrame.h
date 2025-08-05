// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitDelayOneFrame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitDelayOneFrameDelegate); 

/**
 * Delay One Frame (Tick) Ability Task 
 */
UCLASS()
class FPSTEMPLATE_API UWaitDelayOneFrame : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UWaitDelayOneFrame(const FObjectInitializer& ObjectInitializer); 

	UPROPERTY(BlueprintAssignable)
	FWaitDelayOneFrameDelegate OnFinish; 

	virtual void Activate() override; 

	// Wait One Frame
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWaitDelayOneFrame* WaitDelayOneFrame(UGameplayAbility* OwningAbility); 
	
private:
	void OnDelayFinish(); 
};
