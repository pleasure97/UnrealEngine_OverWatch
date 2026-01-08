// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "OWGameplayTags.h"
#include "PlayerHealthBarPool.generated.h"

class UHealthBar;
class UBorder;
class UHorizontalBox;
class UOWAbilitySystemComponent;
class UOWAttributeSet;
class AOWPlayerState; 
struct FOnAttributeChangeData;
struct FGameplayAttribute;

#define DEFINE_LAYOUT_ATTRIBUTE_FUNC(AttributeName) \
    UFUNCTION() \
    void Update##AttributeName##Bars(float NewValue) \
    { \
        if (const FPlayerHealthBarPoolInfo* Info = TagsToHealthBarInfos.Find(FOWGameplayTags::Get().Attributes_Defense_##AttributeName)) \
        { \
            InitializeProgressBars(NewValue, *Info); \
            UpdateBorderVisibility(); \
            DistributeFillSize(); \
        } \
    }

#define DEFINE_VALUE_ATTRIBUTE_FUNC(AttributeName) \
    UFUNCTION() \
    void Update##AttributeName##Bars(float NewValue) \
    { \
        if (const FPlayerHealthBarPoolInfo* Info = TagsToHealthBarInfos.Find(FOWGameplayTags::Get().Attributes_Defense_##AttributeName)) \
        { \
            UpdateProgressBars(NewValue, *Info); \
        } \
    }																				

USTRUCT(BlueprintType)
struct FPlayerHealthBarPoolInfo
{
	GENERATED_BODY()

	FPlayerHealthBarPoolInfo() {}

	FPlayerHealthBarPoolInfo(
		UBorder* InBorder, 
		UHorizontalBox* InHorizontalBox, 
		const FLinearColor& InHealthBarColor, 
		void (UPlayerHealthBarPool::*InUpdateFunc)(float) = nullptr)
		: Border(InBorder), HorizontalBox(InHorizontalBox), HealthBarColor(InHealthBarColor), UpdateFunc(InUpdateFunc) {}

	UPROPERTY()
	TObjectPtr<UBorder> Border;

	UPROPERTY()
	TObjectPtr<UHorizontalBox> HorizontalBox;

	UPROPERTY()
	FLinearColor HealthBarColor;

	// Member Function Pointer of Health Bar Pool 
	void (UPlayerHealthBarPool::* UpdateFunc)(float) = nullptr;

	void ExecuteUpdate(UPlayerHealthBarPool* HealthBarPool, float NewValue) const
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
class FPSTEMPLATE_API UPlayerHealthBarPool : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthBar> HealthBarClass;

	UPROPERTY(BlueprintReadOnly)
	float HealthPerBar = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FLinearColor> HealthBarColors;

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

	/* Health Bar Information */
	UPROPERTY(Transient)
	TMap<FGameplayTag, FPlayerHealthBarPoolInfo> TagsToHealthBarInfos;

	UPROPERTY()
	TArray<FPlayerHealthBarPoolInfo> HealthBarInfos;

	/* Binding Attributes */
	void BindDefensiveAttributeChange(AOWPlayerState* NewPlayerState);

	void BindDefensiveAttributeChange(UOWAbilitySystemComponent* NewAbilitySystemComponent);

	void TryBindAttributes(AOWPlayerState* InOWPlayerState);

	void TryBindAttributes(UOWAbilitySystemComponent* InAbilitySystemComponent);

	bool CanBindAttributes(AOWPlayerState* InOWPlayerState);

	bool CanBindAttributes(UOWAbilitySystemComponent* InAbilitySystemComponent);

	/* Update Attributes */

	UFUNCTION()
	void OnDefensiveAttributeChanged(FGameplayTag AttributeTag, float NewValue);

	DEFINE_LAYOUT_ATTRIBUTE_FUNC(MaxHealth)
	DEFINE_LAYOUT_ATTRIBUTE_FUNC(MaxArmor)
	DEFINE_LAYOUT_ATTRIBUTE_FUNC(MaxShield)
	DEFINE_LAYOUT_ATTRIBUTE_FUNC(TempArmor)
	DEFINE_LAYOUT_ATTRIBUTE_FUNC(TempShield)
	DEFINE_LAYOUT_ATTRIBUTE_FUNC(OverHealth)

	DEFINE_VALUE_ATTRIBUTE_FUNC(Health)
	DEFINE_VALUE_ATTRIBUTE_FUNC(Armor)
	DEFINE_VALUE_ATTRIBUTE_FUNC(Shield)
	/* End Update Attributes */

	void SetIsEnemy(bool InbEnemy); 

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	void InitializeHealthBarPoolInfos();

private:
	/* Update Progress Bars */
	void InitializeProgressBars(const float& NewValue, const FPlayerHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateProgressBars(const float& NewValue, const FPlayerHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateBorderVisibility();
	void DistributeFillSize();
	void ClearHealthBarPool();

	/* Binding Attributes */
	bool ClearRetryTimer();

	TWeakObjectPtr<UOWAbilitySystemComponent> WeakOwnerASC;

	TWeakObjectPtr<UOWAttributeSet> WeakOwnerAS;

	TWeakObjectPtr<AOWPlayerState> WeakOwnerPS;

	FTimerHandle RetryTimerHandle;
};