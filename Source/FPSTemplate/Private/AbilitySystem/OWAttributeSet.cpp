// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "OWGameplayTags.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"

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

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ShieldRegeneration, GetShieldRegenerationAttribute);

	/* Resistance Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Laser, GetLaserResistanceAttribute); 

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

	/* Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, ShieldRegeneration, COND_None, REPNOTIFY_Always); 

	/* Resistance Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, LaserResistance, COND_None, REPNOTIFY_Always); 

	/* Skill Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, SkillGauge, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, UltimateGauge, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, MaxUltimateGauge, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumCurrentBullets, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumMaxBullets, COND_None, REPNOTIFY_Always); 
}

void UOWAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute, NewValue); 

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth()); 
	}
	if (Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxArmor()); 
	}
	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxShield()); 
	}
}

void UOWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data); 

	FEffectProperties EffectProperties; 
	SetEffectProperties(Data, EffectProperties);

	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(EffectProperties.TargetCharacter)) return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth())); 
	}
	if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(), 0.f, GetMaxArmor())); 
	}
	if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.f, GetMaxShield())); 
	}
}

void UOWAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue); 

	if (Attribute == GetMaxHealthAttribute())
	{
		SetHealth(GetMaxHealth()); 
	}
	if (Attribute == GetMaxArmorAttribute())
	{
		SetArmor(GetMaxArmor()); 
	}
	if (Attribute == GetMaxShieldAttribute())
	{
		SetShield(GetMaxShield()); 
	}
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

void UOWAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UOWAttributeSet::OnRep_ShieldRegeneration(const FGameplayAttributeData& OldShieldRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, ShieldRegeneration, OldShieldRegeneration);
}

void UOWAttributeSet::OnRep_LaserResistance(const FGameplayAttributeData& OldLaserResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, LaserResistance, OldLaserResistance);
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

void UOWAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const
{
	// Source = Causer of the Effect
	// Target = Target of the Effect, Owner of the Attribute Set 

	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext(); 
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent(); 

	if (IsValid(EffectProperties.SourceASC) && 
		EffectProperties.SourceASC->AbilityActorInfo.IsValid() && 
		EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get(); 
		EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get(); 
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController(); 
			}
		}
		if (EffectProperties.SourceController != nullptr)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn()); 
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get(); 
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get(); 
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor); 
		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor); 
	}
}