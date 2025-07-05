// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AutoReload.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAutoReload : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CheckInterval = 0.25f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EventDelayInterval = 3.f;

	UFUNCTION(BlueprintCallable)
	bool CheckBulletsState(); 

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override; 

private:
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec); 
};
