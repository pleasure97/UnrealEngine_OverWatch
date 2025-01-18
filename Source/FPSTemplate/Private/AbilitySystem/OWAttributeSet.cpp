// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "OWGameplayTags.h"

UOWAttributeSet::UOWAttributeSet()
{
	InitHealth(50.f); 
	InitMaxHealth(200.f); 
	InitArmor(200.f);
	InitShield(200.f); 

	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	/* Defensive Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_Health, GetHealthAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_MaxHealth, GetMaxHealthAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_OverHealth, GetOverHealthAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_Armor, GetArmorAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_TempArmor, GetTempArmorAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_Shield, GetShieldAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense_TempShield, GetTempShieldAttribute); 
}

void UOWAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, Health, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TempArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TempShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, OverHealth, COND_None, REPNOTIFY_Always);
}

void UOWAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, Health, OldHealth); 
}

void UOWAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, MaxHealth, OldMaxHealth);
}

void UOWAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, Armor, OldArmor);
}

void UOWAttributeSet::OnRep_TempArmor(const FGameplayAttributeData& OldTempArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TempArmor, OldTempArmor);
}

void UOWAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, Shield, OldShield);
}

void UOWAttributeSet::OnRep_TempShield(const FGameplayAttributeData& OldTempShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TempShield, OldTempShield);
}

void UOWAttributeSet::OnRep_OverHealth(const FGameplayAttributeData& OldOverHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, OverHealth, OldOverHealth);
}
