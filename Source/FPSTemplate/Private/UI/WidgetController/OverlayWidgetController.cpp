// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/Data/DefensiveAttributeInfo.h"

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
}

void UOverlayWidgetController::BroadcastDefensiveAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAttributeDefensiveInfo Info = DefensiveAttributeInfo->FindDefensiveAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info); 
}

