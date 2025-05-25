// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AutoRespawn.generated.h"

struct FHeroRespawnInfo;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAutoRespawn : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
private:
	UFUNCTION()
	void OnHeroRespawned(FGameplayTag Channel, const FHeroRespawnInfo& Payload);

	bool IsAvatarDeadOrDying(); 

	UPROPERTY()
	bool IsListeningForReset = false; 

	UPROPERTY()
	bool HasDeadOrDyingAvatar = false; 
};
