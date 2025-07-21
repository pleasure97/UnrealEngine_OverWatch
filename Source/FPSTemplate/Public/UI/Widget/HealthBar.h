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
	// Must Declare Bound Widget Animations before Any Other Widgets 
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ProgressZeroAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	void UpdateProgressBar(const FLinearColor& FillColor, const float& NewPercentValue); 

protected:
	virtual void NativeDestruct() override; 

private:
	float CurrentPercentValue = 1.f;
};
