// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent; 
struct FGameplayEffectSpec; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDurationChanged, FGameplayTag, DurationTag, float, TimeRemaining, float, Duration);

/**
 * 
 */

UCLASS(Abstract)
class FPSTEMPLATE_API UWaitForDurationEffectChange : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationBegin;

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationEnd;

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationTimeUpdated;

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTagContainer DurationTags;
	float DurationInterval = 0.025f;
	bool bUseServerCooldown;
	const UObject* WorldContext;

	void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* InTargetASC, const FGameplayEffectSpec& InSpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	void DurationTagChanged(const FGameplayTag InDurationTag, int32 InNewCount);

	bool GetCooldownRemainingForTag(const FGameplayTagContainer& InDurationTags, float& TimeRemaining, float& InDuration) const;

	UFUNCTION()
	void OnDurationUpdate();

private:
	FTimerHandle DurationTimeUpdateTimerHandle;
};

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class FPSTEMPLATE_API UWaitDurationChange : public UWaitForDurationEffectChange
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitDurationChange* WaitDurationChange(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayTagContainer InDurationTags, float InDurationInterval = 0.05f, bool bInUseServerCooldown = true);

};

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class FPSTEMPLATE_API UWaitCooldownChange : public UWaitForDurationEffectChange
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayTagContainer InCooldownTags, float InDurationInterval = 1.0f, bool bInUseServerCooldown = true);
};