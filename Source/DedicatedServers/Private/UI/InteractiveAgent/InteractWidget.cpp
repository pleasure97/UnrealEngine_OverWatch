// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractiveAgent/InteractWidget.h"

void UInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized(); 

	// Set Pop Up Timer which Calls PlayPopUpAnimation() every 3 Seconds
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(PopUpTimerHandle, this, &UInteractWidget::PlayPopUpAnimation, 3.f, true, 0.f);
	}
}

void UInteractWidget::NativeDestruct()
{
	// Clear Pop Up Timer Handle
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PopUpTimerHandle);
	}
	Super::NativeDestruct();
}

void UInteractWidget::PlayPopUpAnimation()
{
	if (PopUpAnimation)
	{
		PlayAnimation(PopUpAnimation);
	}
}


