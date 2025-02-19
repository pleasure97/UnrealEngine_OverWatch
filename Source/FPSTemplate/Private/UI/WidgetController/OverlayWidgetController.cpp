// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/Data/HealthBarInfo.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "OWGameplayTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	check(HealthBarInfo);
	check(HeroInfo); 

	for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : GetOW_AS()->TagsToAttributes)
	{
		if (Pair.Key.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Defense"))))
		{
			BroadcastHealthBarInfo(Pair.Key, Pair.Value());
		}
	}

	/* Attribute Delgate - Defensive Attribute */
	OnHealthChanged.Broadcast(GetOW_AS()->GetHealth()); 
	OnMaxHealthChanged.Broadcast(GetOW_AS()->GetMaxHealth()); 
	OnArmorChanged.Broadcast(GetOW_AS()->GetArmor()); 
	OnMaxArmorChanged.Broadcast(GetOW_AS()->GetMaxArmor()); 
	OnShieldChanged.Broadcast(GetOW_AS()->GetShield()); 
	OnMaxShieldChanged.Broadcast(GetOW_AS()->GetMaxShield()); 
	OnTempArmorChanged.Broadcast(GetOW_AS()->GetTempArmor()); 
	OnTempShieldChanged.Broadcast(GetOW_AS()->GetTempShield()); 
	OnOverHealthChanged.Broadcast(GetOW_AS()->GetOverHealth()); 

	/* Attribute Delegate - Skills */
	OnUltimateGaugeChanged.Broadcast(GetOW_AS()->GetUltimateGauge()); 
	OnMaxUltimateGaugeChanged.Broadcast(GetOW_AS()->GetMaxUltimateGauge()); 
	OnNumCurrentBulletsChanged.Broadcast(GetOW_AS()->GetNumCurrentBullets()); 
	OnNumMaxBulletsChanged.Broadcast(GetOW_AS()->GetNumMaxBullets()); 
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	/* Ability Delegate */
	if (GetOW_ASC() && GetOW_ASC()->bDefaultAbilitiesGiven)
	{
		BroadcastHeroInfo(); 
	}

	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetHealthAttribute(), OnHealthChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetMaxHealthAttribute(), OnMaxHealthChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetArmorAttribute(), OnArmorChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetMaxArmorAttribute(), OnMaxArmorChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetShieldAttribute(), OnShieldChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetMaxShieldAttribute(), OnMaxShieldChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetTempArmorAttribute(), OnTempArmorChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetTempShieldAttribute(), OnTempShieldChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetOverHealthAttribute(), OnOverHealthChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetUltimateGaugeAttribute(), OnUltimateGaugeChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetMaxUltimateGaugeAttribute(), OnMaxUltimateGaugeChanged); 
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetNumCurrentBulletsAttribute(), OnNumCurrentBulletsChanged);
	BindAttributeChange(AbilitySystemComponent, GetOW_AS()->GetNumMaxBulletsAttribute(), OnNumMaxBulletsChanged);


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
}

void UOverlayWidgetController::BindAttributeChange(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature& Delegate)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[&Delegate](const FOnAttributeChangeData& Data)
		{
			Delegate.Broadcast(Data.NewValue);
		}
	);
}

void UOverlayWidgetController::BroadcastHealthBarInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FBarInfo Info = HealthBarInfo->FindHealthBarInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnUpdateHealthBars.Broadcast(Info); 
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
