// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "HealthBarPool.generated.h"

class UHealthBar;
class USizeBox; 
class UBorder;
class UHorizontalBox;

namespace HealthBarColors
{
	constexpr FLinearColor None(0.f, 0.f, 0.f, 0.f); 
	constexpr FLinearColor White(1.f, 1.f, 1.f, 1.f);
	constexpr FLinearColor Yellow(1.f, 0.8588f, 0.1804f, 1.f);
	constexpr FLinearColor Sky(0.1765f, 0.6706f, 0.9608f, 1.f);
	constexpr FLinearColor Orange(0.9568f, 0.2067f, 0.0439f, 1.f);
	constexpr FLinearColor Blue(0.f, 0.1647f, 0.7843f, 1.f);
	constexpr FLinearColor Green(0.2392f, 0.8941f, 0.2392f, 1.f);
}

USTRUCT()
struct FHealthBarPoolInfo
{
	GENERATED_BODY()

	FHealthBarPoolInfo() {}
	
	FHealthBarPoolInfo(UBorder* InBorder, UHorizontalBox* InHorizontalBox, FLinearColor InHealthBarColor)
		: Border(InBorder)
		, HorizontalBox(InHorizontalBox)
		, HealthBarColor(InHealthBarColor){}

	UPROPERTY()
	TObjectPtr<UBorder> Border; 

	UPROPERTY()
	TObjectPtr<UHorizontalBox> HorizontalBox; 

	UPROPERTY()
	FLinearColor HealthBarColor; 
};
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthBarPool : public UOWUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthBar> HealthBarClass;

	UPROPERTY(BlueprintReadOnly)
	float HealthPerBar = 25.f;

	/* Border */

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Armor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_TempArmor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Shield;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_TempShield;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_OverHealth;

	/* Horizontal Box */

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Armor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_TempArmor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Shield;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_TempShield;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_OverHealth; 

	UPROPERTY()
	TMap<FGameplayTag, FHealthBarPoolInfo> TagsToHealthBarInfos;

	UPROPERTY()
	TArray<FHealthBarPoolInfo> HealthBarInfos; 

	UFUNCTION()
	void BindWidgetControllerEvents();
protected:
	virtual void NativeConstruct() override;
	void InitializeHealthBarPoolInfos();

private:
	/* Update Attributes */
	UFUNCTION()
	void UpdateHealthBars(float NewValue);

	UFUNCTION()
	void UpdateMaxHealthBars(float NewValue);

	UFUNCTION()
	void UpdateArmorBars(float NewValue);

	UFUNCTION()
	void UpdateMaxArmorBars(float NewValue);

	UFUNCTION()
	void UpdateShieldBars(float NewValue);

	UFUNCTION()
	void UpdateMaxShieldBars(float NewValue);

	UFUNCTION()
	void UpdateTempArmorBars(float NewValue);

	UFUNCTION()
	void UpdateTempShieldBars(float NewValue);

	UFUNCTION()
	void UpdateOverHealthBars(float NewValue);

	/* End Update Attributes */

	void InitializeProgressBars(const float& NewValue, const FHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateProgressBars(const float& NewValue, const FHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateBorderVisibility();
	void DistributeFillSize(); 
};
