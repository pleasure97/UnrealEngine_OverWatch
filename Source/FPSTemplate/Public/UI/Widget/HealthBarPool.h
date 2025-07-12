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

USTRUCT(BlueprintType)
struct FHealthBarPoolInfo
{
	GENERATED_BODY()

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FLinearColor> HealthBarColors;

	UPROPERTY()
	TMap<FGameplayTag, FHealthBarPoolInfo> TagsToHealthBarInfos;

	UPROPERTY()
	TArray<FHealthBarPoolInfo> HealthBarInfos; 

	UFUNCTION()
	void BindWidgetControllerEvents();

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

	void SetHealthBarColor(FLinearColor Color); 
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override; 
	void InitializeHealthBarPoolInfos();

private:
	void InitializeProgressBars(const float& NewValue, const FHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateProgressBars(const float& NewValue, const FHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateBorderVisibility();
	void DistributeFillSize(); 
	void ClearHealthBarPool(); 
};
