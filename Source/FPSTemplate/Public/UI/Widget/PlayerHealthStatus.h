// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerHealthStatus.generated.h"

class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerHealthStatus : public UOWUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CurrentHealth; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MaxHealth; 

	UPROPERTY()
	TMap<FGameplayTag, float> CurrentHealthStatusMap;

	UPROPERTY()
	TMap<FGameplayTag, float> MaxHealthStatusMap;

	UPROPERTY()
	float CurrentHealth = 0.f; 
	
	UPROPERTY()
	float MaxHealth = 0.f; 
protected:
	//virtual void NativePreConstruct() override; 
	virtual void NativeConstruct() override; 
	
private:

	void SetCurrentHealth(); 

	void SetMaxHealth(); 

	/* Callback Functions for Delegates of Overlay Widget Controller*/
	UFUNCTION()
	void UpdateCurrentHealthStatus(float NewValue);

	UFUNCTION()
	void UpdateMaxHealthStatus(float NewValue);

	UFUNCTION()
	void UpdateCurrentArmorStatus(float NewValue);

	UFUNCTION()
	void UpdateMaxArmorStatus(float NewValue);

	UFUNCTION()
	void UpdateCurrentShieldStatus(float NewValue);

	UFUNCTION()
	void UpdateMaxShieldStatus(float NewValue);

	UFUNCTION()
	void UpdateTempArmorStatus(float NewValue);

	UFUNCTION()
	void UpdateTempShieldStatus(float NewValue);

	UFUNCTION()
	void UpdateOverHealthStatus(float NewValue);
};
