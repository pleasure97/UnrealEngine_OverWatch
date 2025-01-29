// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct(); 

	/*if (ProgressBar && Texture2D_ProgressBar)
	{
		FProgressBarStyle ProgressBarStyle = ProgressBar->WidgetStyle;

		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(Texture2D_ProgressBar);
		BackgroundBrush.TintColor = FSlateColor(Tint_Background);
		BackgroundBrush.DrawAs = ESlateBrushDrawType::Box; 
		ProgressBarStyle.BackgroundImage = BackgroundBrush;

		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(Texture2D_ProgressBar);
		FillBrush.TintColor = FSlateColor(Tint_Fill);
		FillBrush.DrawAs = ESlateBrushDrawType::Box;
		ProgressBarStyle.FillImage = FillBrush;

		ProgressBar->SetWidgetStyle(ProgressBarStyle);
	}*/
}

void UHealthBar::UpdateProgressBar(const FLinearColor& FillColor, const float& AttributeValue)
{

}

void UHealthBar::PlayProgressZeroAnimation()
{
	if (ProgressZeroAnimation)
	{
		PlayAnimation(ProgressZeroAnimation); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProgressZeroAnimation not found!"));
	}
}

