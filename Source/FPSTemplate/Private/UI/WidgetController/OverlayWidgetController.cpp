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

	OnMaxHealthChanged.Broadcast(GetOW_AS()->GetMaxHealth());
	OnMaxArmorChanged.Broadcast(GetOW_AS()->GetMaxArmor());
	OnMaxShieldChanged.Broadcast(GetOW_AS()->GetMaxShield());
	OnHealthChanged.Broadcast(GetOW_AS()->GetHealth());
	OnArmorChanged.Broadcast(GetOW_AS()->GetArmor());
	OnShieldChanged.Broadcast(GetOW_AS()->GetShield());
	OnTempArmorChanged.Broadcast(GetOW_AS()->GetTempArmor());
	OnTempShieldChanged.Broadcast(GetOW_AS()->GetTempShield());
	OnOverHealthChanged.Broadcast(GetOW_AS()->GetOverHealth());
	OnUltimateGaugeChanged.Broadcast(GetOW_AS()->GetUltimateGauge()); 
	OnMaxUltimateGaugeChanged.Broadcast(GetOW_AS()->GetMaxUltimateGauge()); 
	OnNumCurrentBulletsChanged.Broadcast(GetOW_AS()->GetNumCurrentBullets()); 
	OnNumMaxBulletsChanged.Broadcast(GetOW_AS()->GetNumMaxBullets()); 
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (GetOW_ASC())
	{
		BroadcastHeroInfo();
	}

	for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : GetOW_AS()->TagsToAttributes)
	{
		FOnAttributeChangedSignature* AttributeDelegate = GetDelegateForTag(Pair.Key);
		BindAttributeChange(AbilitySystemComponent, Pair.Key, Pair.Value(), *AttributeDelegate);
	}
}

FOnAttributeChangedSignature* UOverlayWidgetController::GetDelegateForTag(const FGameplayTag& Tag)
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	if (Tag == GameplayTags.Attributes_Defense_Health) return &OnHealthChanged;
	if (Tag == GameplayTags.Attributes_Defense_MaxHealth) return &OnMaxHealthChanged;
	if (Tag == GameplayTags.Attributes_Defense_Armor) return &OnArmorChanged;
	if (Tag == GameplayTags.Attributes_Defense_MaxArmor) return &OnMaxArmorChanged;
	if (Tag == GameplayTags.Attributes_Defense_Shield) return &OnShieldChanged;
	if (Tag == GameplayTags.Attributes_Defense_MaxShield) return &OnMaxShieldChanged;
	if (Tag == GameplayTags.Attributes_Defense_TempArmor) return &OnTempArmorChanged;
	if (Tag == GameplayTags.Attributes_Defense_TempShield) return &OnTempShieldChanged;
	if (Tag == GameplayTags.Attributes_Skill_UltimateGauge) return &OnUltimateGaugeChanged;
	if (Tag == GameplayTags.Attributes_Skill_MaxUltimateGauge) return &OnMaxUltimateGaugeChanged;
	if (Tag == GameplayTags.Attributes_Skill_NumCurrentBullets) return &OnNumCurrentBulletsChanged;
	if (Tag == GameplayTags.Attributes_Skill_NumMaxBullets) return &OnNumMaxBulletsChanged;

	return nullptr; 
}

void UOverlayWidgetController::BindAttributeChange(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature& Delegate)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[&](const FOnAttributeChangeData& Data)
		{
			Delegate.Broadcast(Data.NewValue);
		}
	);
}

void UOverlayWidgetController::BroadcastHeroInfo() const
{
	// EHeroName HeroName = UOWAbilitySystemLibrary::GetHeroName(this); 
	EHeroName HeroName = EHeroName::ILLIARI; 
	const FOWHeroInfo& OWHeroInfo = HeroInfo->HeroInformation[HeroName]; 
	for (const FOWAbilityInfo& OWAbilityInfo : OWHeroInfo.Abilities)
	{
		AbilityInfoDelegate.Broadcast(OWAbilityInfo);
	}
}
