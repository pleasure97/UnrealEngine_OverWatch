// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/Data/DefensiveAttributeInfo.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "OWGameplayTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	check(DefensiveAttributeInfo); 

	for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : GetOW_AS()->TagsToAttributes)
	{
		BroadcastDefensiveAttributeInfo(Pair.Key, Pair.Value()); 
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	check(DefensiveAttributeInfo); 
	for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : GetOW_AS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastDefensiveAttributeInfo(Pair.Key, Pair.Value());			
			}
		);
	}

	if (GetOW_ASC())
	{
		GetOW_ASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped); 
		if (GetOW_ASC()->bDefaultAbilitiesGiven)
		{
			BroadcastHeroInfo(); 
		}
		else
		{
			GetOW_ASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastHeroInfo); 
		}
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const
{
	const FOWGameplayTags& OWGameplayTags = FOWGameplayTags::Get(); 

	EHeroName HeroName = UOWAbilitySystemLibrary::GetHeroName(this); 
	FOWAbilityInfo OWAbilityInfo = HeroInfo->FindAbilityInfoForTag(HeroName, AbilityTag); 
	OWAbilityInfo.StatusTag = StatusTag; 
	AbilityInfoDelegate.Broadcast(OWAbilityInfo); 
}

void UOverlayWidgetController::BroadcastDefensiveAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAttributeDefensiveInfo Info = DefensiveAttributeInfo->FindDefensiveAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info); 
}
