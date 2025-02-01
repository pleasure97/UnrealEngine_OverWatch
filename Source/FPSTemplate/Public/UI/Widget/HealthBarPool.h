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
struct FAttributeDefensiveInfo; 


USTRUCT()
struct FBorderAndHorizontalBox
{
	GENERATED_BODY()

	FBorderAndHorizontalBox() {}
	
	FBorderAndHorizontalBox(UBorder* InBorder, UHorizontalBox* InHorizontalBox)
		: Border(InBorder), HorizontalBox(InHorizontalBox) {}

	UPROPERTY()
	TObjectPtr<UBorder> Border; 

	UPROPERTY()
	TObjectPtr<UHorizontalBox> HorizontalBox; 
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
	TMap<FGameplayTag, FBorderAndHorizontalBox> TagsToBorderAndHorizontalBox;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void UpdateProgressBars(const FAttributeDefensiveInfo& Info);

	void UpdateBorderVisibility();

	void DistributeFillSize(); 
};
