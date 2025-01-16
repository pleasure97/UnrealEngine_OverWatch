// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UOWAttributeSet* OWAttributeSet = CastChecked<UOWAttributeSet>(AttributeSet); 

	OnHealthChanged.Broadcast(OWAttributeSet->GetHealth()); 
	OnMaxHealthChanged.Broadcast(OWAttributeSet->GetMaxHealth()); 
	OnArmorChanged.Broadcast(OWAttributeSet->GetArmor()); 
	OnTempArmorChanged.Broadcast(OWAttributeSet->GetTempArmor()); 
	OnShieldChanged.Broadcast(OWAttributeSet->GetShield()); 
	OnTempShieldChanged.Broadcast(OWAttributeSet->GetTempShield()); 
	OnOverHealthChanged.Broadcast(OWAttributeSet->GetOverHealth()); 
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UOWAttributeSet* OWAttributeSet = CastChecked<UOWAttributeSet>(AttributeSet); 

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged); 

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetArmorAttribute()).AddUObject(this, &UOverlayWidgetController::ArmorChanged); 

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetTempArmorAttribute()).AddUObject(this, &UOverlayWidgetController::TempArmorChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetShieldAttribute()).AddUObject(this, &UOverlayWidgetController::ShieldChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetTempShieldAttribute()).AddUObject(this, &UOverlayWidgetController::TempShieldChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetOverHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OverHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue); 
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue); 
}

void UOverlayWidgetController::ArmorChanged(const FOnAttributeChangeData& Data) const
{
	OnArmorChanged.Broadcast(Data.NewValue); 
}

void UOverlayWidgetController::TempArmorChanged(const FOnAttributeChangeData& Data) const
{
	OnTempArmorChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ShieldChanged(const FOnAttributeChangeData& Data) const
{
	OnShieldChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::TempShieldChanged(const FOnAttributeChangeData& Data) const
{
	OnTempShieldChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OverHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnOverHealthChanged.Broadcast(Data.NewValue); 
}
