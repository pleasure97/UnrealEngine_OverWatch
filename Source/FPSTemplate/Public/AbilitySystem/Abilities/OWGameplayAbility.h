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
	/* Ability Stacking */
	UPROPERTY(EditDefaultsOnly, Category = "OWGameplayAbility|Input")
	FGameplayTag DefaultInputTag; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability Stacking")
	int32 MaxStacks = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability Stacking")
	EAbilityStackingSlot AbilityStackingSlot = EAbilityStackingSlot::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability Stacking")
	float StackRechargeDuration = -1.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability Stacking")
	FGameplayAttribute MaxStackAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability Stacking")
	FGameplayAttribute CurrentStackAttribute;

	virtual void OnHeroSet();

	/* Ability Batching */
	UFUNCTION(BlueprintCallable)
	virtual bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool bEndAbilityImmediately); 

	virtual void ExternalEndAbility(); 

	/* Ability User Widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability UI")
	TSubclassOf<UUserWidget> AbilityWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability UI")
	FVector2D AbilityWidgetSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWGameplayAbility|Ability UI")
	FVector2D AbilityWidgetPosition;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> AbilityWidget;

protected:
	float GetSkillCost(float InLevel = 1.f) const; 

	float GetCooldown(float InLevel = 1.f); 

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPawnAvatarSet")
	void K2_OnHeroSet();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags, 
		OUT FGameplayTagContainer* OptionalRelevantTags) const override;

	void InitializeAbilityStacking(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	void InitializeAbilityUI(const FGameplayAbilityActorInfo* ActorInfo);
};
