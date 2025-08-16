// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/OWHUD.h"
#include "UI/Widget/OWUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/PlayerOverlay.h"

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

	PlayerOverlay = CreateWidget<UPlayerOverlay>(PC, OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS); 
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams); 

	PlayerOverlay->SetWidgetController(WidgetController); 

	if (!bInitialized)
	{
		bInitialized = true; 
		PlayerOverlay->AddToViewport();
		WidgetController->BroadcastInitialValues();
	}
}

void AOWHUD::CollapseOverlay()
{
	if (IsValid(PlayerOverlay))
	{
		PlayerOverlay->SetVisibility(ESlateVisibility::Collapsed); 
	}
}

void AOWHUD::RemoveOverlay()
{
	if (IsValid(PlayerOverlay))
	{
		PlayerOverlay->RemoveFromParent(); 
		PlayerOverlay = nullptr; 
	}
}
