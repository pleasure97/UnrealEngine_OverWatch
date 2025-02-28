// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HealthBar.generated.h"

class UHorizontalBox; 
class UProgressBar; 
class UImage; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealthBar : public UOWUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	TObjectPtr<UTexture2D> Texture2D_ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	FLinearColor Tint_Background = FLinearColor(0.3058f, 0.3058f, 0.3058f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	FLinearColor Tint_Fill;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ProgressZeroAnimation;

	UPROPERTY()
	float CurrentPercentValue = 1.f; 

	void UpdateProgressBar(const FLinearColor& FillColor, const float& NewPercentValue); 

	UFUNCTION()
	void PlayProgressZeroAnimation();};
