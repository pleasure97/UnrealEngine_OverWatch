// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OWGameplayAbility.generated.h"

UENUM()
enum class EAbilityStackingSlot : uint8
{
	None, 
	FirstSkill
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag DefaultInputTag; 

	UPROPERTY(EditDefaultsOnly, Category = "Ability Stacking")
	int32 MaxStacks = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Stacking")
	EAbilityStackingSlot AbilityStackingSlot = EAbilityStackingSlot::None;

	virtual void OnHeroSet();

	/* Ability Batching */
	UFUNCTION(BlueprintCallable)
	virtual bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool bEndAbilityImmediately); 

	virtual void ExternalEndAbility(); 

protected:
	float GetSkillCost(float InLevel = 1.f) const; 

	float GetCooldown(float InLevel = 1.f); 

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPawnAvatarSet")
	void K2_OnHeroSet();

	void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec); 

	void ApplyStackChangeGameplayEffect(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
};
