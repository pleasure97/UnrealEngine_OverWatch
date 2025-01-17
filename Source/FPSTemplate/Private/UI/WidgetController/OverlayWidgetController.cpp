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

	/* Bind Callbacks of Attributes Changed - Health */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue); 
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetOverHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnOverHealthChanged.Broadcast(Data.NewValue);
			}
		);

	/* Bind Callbacks of Attributes Changed - Armor */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetArmorAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnArmorChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetTempArmorAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnTempArmorChanged.Broadcast(Data.NewValue);
			}
		);

	/* Bind Callbacks of Attributes Changed - Shield */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetShieldAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnShieldChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		OWAttributeSet->GetTempShieldAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnTempShieldChanged.Broadcast(Data.NewValue);
			}
		);
}