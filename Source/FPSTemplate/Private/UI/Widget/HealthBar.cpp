// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBar::NativeDestruct()
{
    StopAllAnimations(); 

    Super::NativeDestruct(); 
}

void UHealthBar::UpdateProgressBar(const FLinearColor& FillColor, const float& NewPercentValue)
{
    if (ProgressBar)
    {
        ProgressBar->SetFillColorAndOpacity(FillColor);
        ProgressBar->SetPercent(NewPercentValue);

        if (CurrentPercentValue > 0.f && NewPercentValue <= 0.f)
        {
            // Reset Progress Zero Animation if Progress Zero Animation was already playing 
            if (ProgressZeroAnimation && IsAnimationPlayingForward(ProgressZeroAnimation))
            {
                StopAnimation(ProgressZeroAnimation);
                SetAnimationCurrentTime(ProgressZeroAnimation, 0.f);
            }

            if (ProgressZeroAnimation)
            {
                PlayAnimationForward(ProgressZeroAnimation, 1.f, true);
            }
        }

        CurrentPercentValue = NewPercentValue;
    }
}