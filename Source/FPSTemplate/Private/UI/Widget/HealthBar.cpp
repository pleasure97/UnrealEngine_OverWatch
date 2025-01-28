// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (ProgressBar && Texture2D_ProgressBar)
	{
		FProgressBarStyle ProgressBarStyle = ProgressBar->WidgetStyle;

		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(Texture2D_ProgressBar);
		BackgroundBrush.TintColor = FSlateColor(Tint_Background);
		ProgressBarStyle.BackgroundImage = BackgroundBrush;

		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(Texture2D_ProgressBar);
		FillBrush.TintColor = FSlateColor(Tint_Fill);
		ProgressBarStyle.FillImage = FillBrush;

		ProgressBar->SetWidgetStyle(ProgressBarStyle);
	}
}

void UHealthBar::UpdateProgressBar(const FLinearColor& FillColor, const float& AttributeValue)
{

}

