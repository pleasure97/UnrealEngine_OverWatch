// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UOWAttributeSet* OWAttributeSet = CastChecked<UOWAttributeSet>(AttributeSet); 

	OnHealthChanged.Broadcast(OWAttributeSet->GetHealth()); 
	OnMaxHealthChanged.Broadcast(OWAttributeSet->GetMaxHealth()); 
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UOWAttributeSet* OWAttributeSet = CastChecked<UOWAttributeSet>(AttributeSet); 

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged); 
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue); 
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue); 
}
