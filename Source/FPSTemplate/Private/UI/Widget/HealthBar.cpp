// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct(); 

	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UHealthBar::PlayProgressZeroAnimation, 1.f, true, 1.f); 
}

void UHealthBar::UpdateProgressBar(const FLinearColor& FillColor, const float& PercentValue)
{
	if (PercentValue == 0.f) PlayProgressZeroAnimation(); 

	ProgressBar->WidgetStyle.FillImage.TintColor = FillColor; 

	ProgressBar->SetPercent(PercentValue); 
}

void UHealthBar::PlayProgressZeroAnimation()
{
	if (ProgressZeroAnimation)
	{
		PlayAnimation(ProgressZeroAnimation); 
	}
}

