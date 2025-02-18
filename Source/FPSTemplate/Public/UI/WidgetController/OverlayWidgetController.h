// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/OWWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FBarInfo; 
class UHealthBarInfo;
struct FGameplayTag; 
struct FGameplayAttribute; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHealthBars, const FBarInfo&, Info); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerSkills, const TArray<FOWAbilityInfo>&, NewPlayerSkills); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerWeaponStatus, const FOWAbilityInfo&, NewWeaponStatus); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateUltimateGauge, float, NewUltimateGauge); 

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

	UPROPERTY(BlueprintAssignable, Category = "UI|Health Bar Pool")	
	FOnUpdateHealthBars OnUpdateHealthBars; 

	UPROPERTY(BlueprintAssignable, Category = "UI|Skills")
	FOnUpdatePlayerSkills OnUpdatePlayerSkills; 

	UPROPERTY(BlueprintAssignable, Category = "UI|Weapon Status")
	FOnUpdatePlayerWeaponStatus OnPlayerWeaponStatus;

	UPROPERTY(BlueprintAssignable, Category = "UI|Ultimate Gauge")
	FOnUpdateUltimateGauge OnUpdateUltimateGauge; 

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UHealthBarInfo> HealthBarInfo; 

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const; 
private:
	void BroadcastHealthBarInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const; 
};
