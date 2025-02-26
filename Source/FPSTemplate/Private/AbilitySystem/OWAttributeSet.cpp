// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "OWGameplayTags.h"

UOWAttributeSet::UOWAttributeSet()
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	/* Defensive Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_MaxArmor, GetMaxArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_MaxShield, GetMaxShieldAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_Shield, GetShieldAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_TempArmor, GetTempArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_TempShield, GetTempShieldAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_OverHealth, GetOverHealthAttribute);

	/* Skill Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Skill_SkillGauge, GetSkillGaugeAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Skill_UltimateGauge, GetUltimateGaugeAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Skill_MaxUltimateGauge, GetMaxUltimateGaugeAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Skill_NumCurrentBullets, GetNumCurrentBulletsAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Skill_NumMaxBullets, GetNumMaxBulletsAttribute); 
}

void UOWAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	/* Defensive Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, MaxArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TempArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TempShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, OverHealth, COND_None, REPNOTIFY_Always);
	
	/* Skill Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, SkillGauge, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, UltimateGauge, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, MaxUltimateGauge, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumCurrentBullets, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumMaxBullets, COND_None, REPNOTIFY_Always); 
}

void UOWAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, MaxHealth, OldMaxHealth);
}

void UOWAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, MaxArmor, OldMaxArmor);
}

void UOWAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, MaxShield, OldMaxShield);
}


void UOWAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, Health, OldHealth); 
}


void UOWAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, Armor, OldArmor);
}

void UOWAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, Shield, OldShield);
}

void UOWAttributeSet::OnRep_TempArmor(const FGameplayAttributeData& OldTempArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TempArmor, OldTempArmor);
}

void UOWAttributeSet::OnRep_TempShield(const FGameplayAttributeData& OldTempShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TempShield, OldTempShield);
}

void UOWAttributeSet::OnRep_OverHealth(const FGameplayAttributeData& OldOverHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, OverHealth, OldOverHealth);
}

void UOWAttributeSet::OnRep_SkillGauge(const FGameplayAttributeData& OldSkillGauge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, SkillGauge, OldSkillGauge); 
}

void UOWAttributeSet::OnRep_UltimateGauge(const FGameplayAttributeData& OldUltimateGauge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, UltimateGauge, OldUltimateGauge);
}

void UOWAttributeSet::OnRep_MaxUltimateGauge(const FGameplayAttributeData& OldMaxUltimateGauge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, MaxUltimateGauge, OldMaxUltimateGauge);
}

void UOWAttributeSet::OnRep_NumCurrentBullets(const FGameplayAttributeData& OldNumCurrentBullets) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, NumCurrentBullets, OldNumCurrentBullets);
}

void UOWAttributeSet::OnRep_NumMaxBullets(const FGameplayAttributeData& OldNumMaxBullets) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, NumMaxBullets, OldNumMaxBullets);
}
