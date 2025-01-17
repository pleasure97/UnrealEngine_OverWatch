// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UHealthBar::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	if (ProgressBar && Image_Background  && Image_Fill)
	{
		FProgressBarStyle ProgressBarStyle = ProgressBar->WidgetStyle; 

		FSlateBrush BackgroundBrush; 
		BackgroundBrush.SetResourceObject(Image_Background); 
		BackgroundBrush.TintColor = FSlateColor(Tint_Background); 
		ProgressBarStyle.BackgroundImage = BackgroundBrush; 

		FSlateBrush FillBrush; 
		FillBrush.SetResourceObject(Image_Fill); 
		FillBrush.TintColor = FSlateColor(Tint_Fill); 
		ProgressBarStyle.FillImage = FillBrush; 

		ProgressBar->SetWidgetStyle(ProgressBarStyle);
	}
}

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct(); 
}

