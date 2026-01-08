// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "OWGameplayTags.h"
#include "HealthBarPool.generated.h"

class UHealthBar;
class USizeBox; 
class UBorder;
class UHorizontalBox;
class UOWAbilitySystemComponent; 
class UOWAttributeSet;
struct FOnAttributeChangeData;
struct FGameplayTag;
struct FGameplayAttribute;

#define DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(AttributeName) \
    UFUNCTION() \
    void Update##AttributeName##Bars(float NewValue) \
    { \
        if (const FHealthBarPoolInfo* Info = TagsToHealthBarInfos.Find(FOWGameplayTags::Get().Attributes_Defense_##AttributeName)) \
        { \
            InitializeProgressBars(NewValue, *Info); \
            UpdateBorderVisibility(); \
            DistributeFillSize(); \
        } \
    }

#define DEFINE_VALUE_ATTRIBUTE_FUNCTION(AttributeName) \
    UFUNCTION() \
    void Update##AttributeName##Bars(float NewValue) \
    { \
        if (const FHealthBarPoolInfo* Info = TagsToHealthBarInfos.Find(FOWGameplayTags::Get().Attributes_Defense_##AttributeName)) \
        { \
            UpdateProgressBars(NewValue, *Info); \
        } \
    }																				

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

	// Member Function Pointer of Health Bar Pool 
	void (UHealthBarPool::* UpdateFunc)(float) = nullptr; 

	void ExecuteUpdate(UHealthBarPool* HealthBarPool, float NewValue) const
	{
		if (HealthBarPool && UpdateFunc)
		{
			(HealthBarPool->*UpdateFunc)(NewValue); 
		}
	}
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
	void BindDefensiveAttributeChange();

	/* Update Attributes */
	UFUNCTION()
	void OnDefensiveAttributeChanged(FGameplayTag AttributeTag, float NewValue); 

	DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(MaxHealth)
	DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(MaxArmor)
	DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(MaxShield)
	DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(TempArmor)
	DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(TempShield)
	DEFINE_LAYOUT_ATTRIBUTE_FUNCTION(OverHealth)

	DEFINE_VALUE_ATTRIBUTE_FUNCTION(Health)
	DEFINE_VALUE_ATTRIBUTE_FUNCTION(Armor)
	DEFINE_VALUE_ATTRIBUTE_FUNCTION(Shield)
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

	TWeakObjectPtr<UOWAbilitySystemComponent> WeakOwnerASC;
	TObjectPtr<UOWAttributeSet> WeakOwnerAS;
};
