// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UHealthBar::NativeConstruct()
{
    Super::NativeConstruct(); 

    if (ProgressZeroAnimation)
    {
        FWidgetAnimationDynamicEvent AnimationFinishedEvent;
        AnimationFinishedEvent.BindUFunction(this, FName("OnProgressZeroAnimationFinished"));
        BindToAnimationFinished(ProgressZeroAnimation, AnimationFinishedEvent);
    }
}

void UHealthBar::NativeDestruct()
{
    if (ProgressZeroAnimation)
    {
        UnbindAllFromAnimationFinished(ProgressZeroAnimation); 
    }

    Super::NativeDestruct(); 
}

void UHealthBar::UpdateProgressBar(const FLinearColor& FillColor, const float& NewPercentValue)
{
    if (CurrentPercentValue > 0.f && NewPercentValue == 0.f)
    {
        FProgressBarStyle Style = ProgressBar->GetWidgetStyle();
        // Set Background Tint Color to Red 
        Style.BackgroundImage.TintColor = FSlateColor(FLinearColor::Red); 
        ProgressBar->SetWidgetStyle(Style);
        if (ProgressZeroAnimation && !IsAnimationPlaying(ProgressZeroAnimation))
        {
            PlayAnimation(ProgressZeroAnimation); 
        }
    }

    if (ProgressBar)
    {
        CurrentPercentValue = NewPercentValue;
        ProgressBar->SetFillColorAndOpacity(FillColor); 
        ProgressBar->SetPercent(NewPercentValue);
    }
}

void UHealthBar::OnProgressZeroAnimationFinished()
{
    if (ProgressBar)
    {
        FProgressBarStyle Style = ProgressBar->WidgetStyle;
        // Reset Background Tint Color to Gray 
        Style.BackgroundImage.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));
        ProgressBar->SetWidgetStyle(Style);
    }
}
