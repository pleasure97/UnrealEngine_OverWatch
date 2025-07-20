// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OWGameplayAbility.generated.h"

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

	virtual void OnHeroSet();

protected:
	float GetSkillCost(float InLevel = 1.f) const; 

	float GetCooldown(float InLevel = 1.f); 

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPawnAvatarSet")
	void K2_OnHeroSet();
};
