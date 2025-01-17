// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HealthBarPool.generated.h"

class UHealthBar;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthBarPool : public UOWUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHealthBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Health;

	UPROPERTY(EditDefaultsOnly)
	float HealthPerBar = 25.f;

	UFUNCTION()
	void OnHealthChanged(float NewValue);

	UFUNCTION()
	void OnMaxHealthChanged(float NewValue); 

	UFUNCTION()
	void OnArmorChanged(float NewValue);

	UFUNCTION()
	void OnTempArmorChanged(float NewValue);

	UFUNCTION()
	void OnShieldChanged(float NewValue);

	UFUNCTION()
	void OnTempShieldChanged(float NewValue);

	UFUNCTION()
	void OnOverHealthChanged(float NewValue);

	UFUNCTION()
	void UpdateProgressBars(float NewValue);
};
