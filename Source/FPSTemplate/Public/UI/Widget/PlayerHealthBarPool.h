// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerHealthBarPool.generated.h"

class UHealthBar;
class UBorder;
class UHorizontalBox;
class UOWAbilitySystemComponent;
class UOWAttributeSet;
class AOWPlayerState; 
struct FOnAttributeChangeData;
struct FGameplayAttribute;

USTRUCT(BlueprintType)
struct FPlayerHealthBarPoolInfo
{
	GENERATED_BODY()

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

	UPROPERTY()
	TMap<FGameplayTag, FPlayerHealthBarPoolInfo> TagsToHealthBarInfos;

	UPROPERTY()
	TArray<FPlayerHealthBarPoolInfo> HealthBarInfos;

	UFUNCTION()
	void BindDefensiveAttributeChange(AOWPlayerState* NewPlayerState);

	/* Update Attributes */
	UFUNCTION()
	void OnDefensiveAttributeChanged(FGameplayTag AttributeTag, float NewValue);

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

	UFUNCTION(BlueprintCallable)
	void SetPlayerState(AOWPlayerState* NewOWPlayerState); 
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void InitializeHealthBarPoolInfos();

private:
	void InitializeProgressBars(const float& NewValue, const FPlayerHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateProgressBars(const float& NewValue, const FPlayerHealthBarPoolInfo& HealthBarPoolInfo);
	void UpdateBorderVisibility();
	void DistributeFillSize();
	void ClearHealthBarPool();

	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> OwnerAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UOWAttributeSet> OwnerAttributeSet;

	UPROPERTY()
	TObjectPtr<AOWPlayerState> OWPlayerState; 
};
