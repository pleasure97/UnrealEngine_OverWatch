// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/OWHUD.h"
#include "UI/Widget/OWUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AOWHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass); 
		OverlayWidgetController->SetWidgetControllerParams(WCParams); 
		OverlayWidgetController->BindCallbacksToDependencies(); 

		return OverlayWidgetController; 
	}
	return OverlayWidgetController; 
}

void AOWHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitialized, please fill out BP_OWHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Uninitialized, please fill out BP_OWHUD")); 

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UOWUserWidget>(Widget); 

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS); 
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams); 

	OverlayWidget->SetWidgetController(WidgetController); 

	Widget->AddToViewport(); 
	WidgetController->BroadcastInitialValues();
}
