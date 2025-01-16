// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorChangedSignature, float, NewArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTempArmorChangedSignature, float, NewTempArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldChangedSignature, float, NewShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTempShieldChangedSignature, float, NewTempShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverHealthChangedSignature, float, NewOverHealth);

struct FOnAttributeChangeData;

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

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged; 

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged; 

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnArmorChangedSignature OnArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnTempArmorChangedSignature OnTempArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnShieldChangedSignature OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnTempShieldChangedSignature OnTempShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnOverHealthChangedSignature OnOverHealthChanged;
	
protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const; 
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const; 
	void ArmorChanged(const FOnAttributeChangeData& Data) const;
	void TempArmorChanged(const FOnAttributeChangeData& Data) const;
	void ShieldChanged(const FOnAttributeChangeData& Data) const;
	void TempShieldChanged(const FOnAttributeChangeData& Data) const;
	void OverHealthChanged(const FOnAttributeChangeData& Data) const;
};
