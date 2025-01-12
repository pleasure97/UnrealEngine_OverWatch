// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UOWAttributeSet* OWAttributeSet = CastChecked<UOWAttributeSet>(AttributeSet); 

	OnHealthChanged.Broadcast(OWAttributeSet->GetHealth()); 
	OnMaxHealthChanged.Broadcast(OWAttributeSet->GetMaxHealth()); 
}
