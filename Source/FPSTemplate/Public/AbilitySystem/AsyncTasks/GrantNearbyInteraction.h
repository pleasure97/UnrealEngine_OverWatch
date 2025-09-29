// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GrantNearbyInteraction.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UGrantNearbyInteraction : public UAbilityTask
{
	GENERATED_BODY()

public:
	/* Wait Until Overlay Occurs. */
	UFUNCTION(BlueprintCallable)
	static UGrantNearbyInteraction* GrantAbilitiesForNearbyInteractors(
		UGameplayAbility* OwningGameplayAbility, float InteractionScanRange, float InteractionScanRate);

protected:
	virtual void Activate() override;

private:
	virtual void OnDestroy(bool bAbilityEnded) override;

	UFUNCTION()
	void QueryInteractables(); 

	float InteractionScanRate = 0.1f; 
	float InteractionScanRange = 100.f; 

	FTimerHandle InteractionQueryTimerHandle; 

	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractionAbilityCache; 
};
