// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "UltimateGauge.generated.h"

class UImage; 
class UTextBlock; 
class UMaterialInstanceDynamic; 

namespace UltimateGaugeColor
{
	constexpr FLinearColor None(0.f, 0.f, 0.f, 0.f);
	constexpr FLinearColor Orange(0.9568f, 0.2067f, 0.044f, 1.f);
	constexpr FLinearColor Blue(0.3686f, 0.9961f, 0.9765f, 1.f);
}


/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UUltimateGauge : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateGauge; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Percent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateIcon;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance; 

	UFUNCTION()
	void ReceiveAbilityInfo(const FOWAbilityInfo& Info);

	UFUNCTION()
	void SetMaxUltimateGauge(float NewValue); 

	UFUNCTION()
	void UpdateUltimateGauge(float NewValue); 

	float Percent = 0.f; 

	float CurrentUltimateGauge = 0.f; 

	float MaxUltimateGauge = 0.f; 

	bool bAlreadyUpdated = false;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override; 
};
