// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "WaitInputReleaseWithTimeOut.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleaseOrTimeOut, float, TimeHeld); 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UWaitInputReleaseWithTimeOut : public UAbilityTask_WaitInputRelease
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability|Input")
	float MaxHoldTime = 0.f; 

	float StartTime = 0.f; 

	UPROPERTY(BlueprintAssignable)
	FOnReleaseOrTimeOut OnReleaseOrTimeOut; 

	virtual void Activate() override; 

	UFUNCTION(BlueprintCallable, Category="Ability|Input", meta = (HideSpawnParams="OwningAbility", DefaultToSelf="OwningAbility"))
	static UWaitInputReleaseWithTimeOut* WaitInputReleaseWithTimeOut(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InMaxHoldTime); 
	
protected:
	FTimerHandle TimeOutHandle; 

	UFUNCTION()
	void OnActualRelease(float TimeHeld); 

	UFUNCTION()
	void OnTimeOutReached(); 
};
