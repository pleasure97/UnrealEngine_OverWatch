// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UHealthBar::UpdateProgressBar(const FLinearColor& FillColor, const float& NewPercentValue)
{
	if (CurrentPercentValue > 0.f && NewPercentValue == 0.f) PlayProgressZeroAnimation(); 

	if (ProgressBar)
	{
		ProgressBar->SetFillColorAndOpacity(FillColor); 
		ProgressBar->SetPercent(NewPercentValue);
	}

	CurrentPercentValue = NewPercentValue; 
}

void UHealthBar::PlayProgressZeroAnimation()
{
	if (ProgressZeroAnimation)
	{
		PlayAnimation(ProgressZeroAnimation); 
	}
}

