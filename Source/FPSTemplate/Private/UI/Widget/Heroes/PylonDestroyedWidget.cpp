// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Heroes/PylonDestroyedWidget.h"

void UPylonDestroyedWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (UWorld* World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &UPylonDestroyedWidget::WidgetDestroyed, TimeWidgetDestroyed, false);
	}
}

void UPylonDestroyedWidget::WidgetDestroyed()
{
	RemoveFromParent();
}
