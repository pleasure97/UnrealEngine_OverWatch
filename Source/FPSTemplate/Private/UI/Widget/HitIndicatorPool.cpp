// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HitIndicatorPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/HitIndicator.h"
#include "Components/Overlay.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Player/OWPlayerController.h"

void UHitIndicatorPool::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Check Hit Indicator Widget Class is not nullptr 
	if (!HitIndicatorClass) { return; }

	check(WidgetController); 

	// Bind Overlay Widget Controller to ProcessDamageReceived() Callback
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 

		if (UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(OverlayWidgetController->AttributeSet))
		{
			OWAttributeSet->OnDamageReceived.AddUObject(this, &UHitIndicatorPool::ProcessDamageReceived); 
		}
	}
}

void UHitIndicatorPool::NativeDestruct()
{
	// Unbind Overlay Widget Controller to All Callbacks 
	if (UOverlayWidgetController* OverlayController = Cast<UOverlayWidgetController>(WidgetController))
	{
		if (UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(OverlayController->AttributeSet))
		{
			OWAttributeSet->OnDamageReceived.RemoveAll(this);
		}
	} 

	Super::NativeDestruct(); 
}

void UHitIndicatorPool::ProcessDamageReceived(AActor* DamageCauser, AActor* OwnerActor, float Damage)
{
	// Check Damage Causer, Owner Actor, and Hit Indicator Class is not nullptr 
	if (!DamageCauser || !OwnerActor || !HitIndicatorClass) { return; }
	
	UHitIndicator* HitIndicator = CreateWidget<UHitIndicator>(GetOwningPlayer(), HitIndicatorClass);
	if (HitIndicator)
	{
		int32 ViewWidth, ViewHeight;
		// Cast Widget Controller to Overlay Widget Controller
		if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
		{
			// Get Player Controller from Overlay widget Controller
			if (AOWPlayerController* OWPlayerController = OverlayWidgetController->GetOW_PC())
			{
				// Get Width and Height of Viewport Size 
				OWPlayerController->GetViewportSize(ViewWidth, ViewHeight); 
				// Set Hit Information of Hit Indicator and Add it to Player Screen 
				HitIndicator->SetHitInfo(DamageCauser, OwnerActor, Damage);
				HitIndicator->AddToPlayerScreen(); 
				// Adjust Alignment and Size of Hit Indicator Literally 
				HitIndicator->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
				HitIndicator->SetDesiredSizeInViewport(FVector2D(100.f, 100.f));
				HitIndicator->SetPositionInViewport(FVector2D(ViewWidth * 0.5f, ViewHeight * 0.5f), true);
			}
		}
	}
}
