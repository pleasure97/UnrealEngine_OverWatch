// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "OWAnimInstance.generated.h"

class UAbilitySystemComponent; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UOWAnimInstance(const FObjectInitializer& ObjectInitializer); 
	
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC); 

protected:
	virtual void NativeInitializeAnimation() override; 

protected:
	// Gameplay tags that can be mapped to blueprint variables. 
	// The variable will automatically update as the tags are added or removed. 
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap; 

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.f; 
};
