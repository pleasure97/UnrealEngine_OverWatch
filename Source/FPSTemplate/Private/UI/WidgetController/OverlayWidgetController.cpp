// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "OWGameplayTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	check(HeroInfo); 

	BroadcastHeroInfo(); 

	for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : GetOW_AS()->TagsToAttributes)
	{
		FOnAttributeChangedSignature& AttributeDelegate = GetDelegateForTag(Pair.Key); 
		BindAttributeChange(AbilitySystemComponent, Pair.Key, Pair.Value(), AttributeDelegate); 
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	/* Ability Delegate */
	if (GetOW_ASC() && GetOW_ASC()->bDefaultAbilitiesGiven)
	{
		BroadcastHeroInfo(); 
	}

	for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : GetOW_AS()->TagsToAttributes)
	{
		FOnAttributeChangedSignature& AttributeDelegate = GetDelegateForTag(Pair.Key);
		BindAttributeChange(AbilitySystemComponent, Pair.Key, Pair.Value(), AttributeDelegate);
	}
}

FOnAttributeChangedSignature& UOverlayWidgetController::GetDelegateForTag(const FGameplayTag& Tag)
{
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_Health) return OnHealthChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_MaxHealth) return OnMaxHealthChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_Armor) return OnArmorChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_MaxArmor) return OnMaxArmorChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_Shield) return OnShieldChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_MaxShield) return OnMaxShieldChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_TempArmor) return OnTempArmorChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_TempShield) return OnTempShieldChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Defense_OverHealth) return OnOverHealthChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Skill_UltimateGauge) return OnUltimateGaugeChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Skill_MaxUltimateGauge) return OnUltimateGaugeChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Skill_NumCurrentBullets) return OnNumCurrentBulletsChanged;
	if (Tag == FOWGameplayTags::Get().Attributes_Skill_NumMaxBullets) return OnNumMaxBulletsChanged;

	return OnHealthChanged; 
}

void UOverlayWidgetController::BindAttributeChange(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature& Delegate)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[&](const FOnAttributeChangeData& Data)
		{
			Delegate.Broadcast(Tag, Data.NewValue);
		}
	);
}

void UOverlayWidgetController::BroadcastHeroInfo() const
{
	EHeroName HeroName = UOWAbilitySystemLibrary::GetHeroName(this); 
	const FOWHeroInfo& OWHeroInfo = HeroInfo->HeroInformation[HeroName]; 
	for (const FOWAbilityInfo& OWAbilityInfo : OWHeroInfo.Abilities)
	{
		AbilityInfoDelegate.Broadcast(OWAbilityInfo);
	}
}
