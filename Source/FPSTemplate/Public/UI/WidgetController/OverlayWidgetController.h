// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FAttributeDefensiveInfo; 
class UDefensiveAttributeInfo;
struct FGameplayTag; 
struct FGameplayAttribute; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAttributeDefensiveInfo&, Info); 

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

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")	
	FAttributeInfoSignature AttributeInfoDelegate; 

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDefensiveAttributeInfo> DefensiveAttributeInfo; 

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const; 
private:
	void BroadcastDefensiveAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const; 
};
