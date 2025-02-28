// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayTag; 
struct FGameplayAttribute; 
class UAbilitySystemComponent; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FOWAbilityInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FPSTEMPLATE_API UOverlayWidgetController : public UOWWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override; 
	virtual void BindCallbacksToDependencies() override; 
	FOnAttributeChangedSignature* GetDelegateForTag(const FGameplayTag& Tag); 

	/* Ability Delegate */
	UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;

	/* Attribute Delegate  - Defensive Attributes */
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged; 

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnTempArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnTempShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnOverHealthChanged;

	/* Attribute Delegate  - Skills */

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnUltimateGaugeChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxUltimateGaugeChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnNumCurrentBulletsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnNumMaxBulletsChanged;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UHeroInfo> HeroInfo;

private:
	void BindAttributeChange(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature& Delegate); 
	void BroadcastHeroInfo() const; 
};
