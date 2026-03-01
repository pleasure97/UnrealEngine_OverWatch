// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OWUserWidget.h"

void UOWUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController; 
	SetChildWidgetControllers(); 
}

void UOWUserWidget::SetChildWidgetControllers()
{
}

void UOWUserWidget::UnregisterGamePhaseListener(FGameplayMessageListenerHandle& Handle)
{
	if (Handle.IsValid())
	{
		Handle.Unregister();
	}
}
