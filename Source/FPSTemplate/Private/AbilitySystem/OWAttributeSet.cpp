// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "OWGameplayTags.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Interface/LevelUpInterface.h"
#include "Interface/OmnicInterface.h"
#include "OWAbilityTypes.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"

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

	/* Match Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Match_NumKills, GetNumKillsAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Match_NumDeaths, GetNumDeathsAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Match_NumAssists, GetNumAssistsAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Match_TotalDamage, GetTotalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Match_TotalHeal, GetTotalHealAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Match_TotalMitigatedDamage, GetTotalMitigatedDamageAttribute);
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

	/* Match Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumKills, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumDeaths, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, NumAssists, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TotalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TotalHeal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOWAttributeSet, TotalMitigatedDamage, COND_None, REPNOTIFY_Always);
}

void UOWAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue); 

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
	if (Attribute == GetNumCurrentBulletsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetNumMaxBullets()); 
	}
	if (Attribute == GetUltimateGaugeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxUltimateGauge()); 
	}
}

bool UOWAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false; 
	}

	// Process All Incoming Damage, whether greater than 0 or less than 0
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		// Find Damage Immune GameplayTag
		if (Data.Target.HasMatchingGameplayTag(FOWGameplayTags::Get().Damage_Immune))
		{
			Data.EvaluatedData.Magnitude = 0.f; 
			return false; 
		}
		// TODO - May need to deal with when damage comes in, even when character's dead?
	}

	return true; 
}

void UOWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data); 

	FEffectProperties EffectProperties; 
	SetEffectProperties(Data, EffectProperties);

	if (EffectProperties.TargetCharacter &&
		EffectProperties.TargetCharacter->Implements<UCombatInterface>() 
		&& ICombatInterface::Execute_IsDead(EffectProperties.TargetCharacter)) return;

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
	if (Data.EvaluatedData.Attribute == GetNumCurrentBulletsAttribute())
	{
		SetNumCurrentBullets(FMath::Clamp(GetNumCurrentBullets(), 0.f, GetNumMaxBullets())); 
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(EffectProperties); 
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(EffectProperties); 
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

void UOWAttributeSet::OnRep_NumKills(const FGameplayAttributeData& OldNumKills) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, NumKills, OldNumKills);
}

void UOWAttributeSet::OnRep_NumDeaths(const FGameplayAttributeData& OldNumDeaths) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, NumDeaths, OldNumDeaths);
}

void UOWAttributeSet::OnRep_NumAssists(const FGameplayAttributeData& OldNumAssists) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, NumAssists, OldNumAssists);
}

void UOWAttributeSet::OnRep_TotalDamage(const FGameplayAttributeData& OldTotalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TotalDamage, OldTotalDamage);
}

void UOWAttributeSet::OnRep_TotalHeal(const FGameplayAttributeData& OldTotalHeal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TotalHeal, OldTotalHeal);
}

void UOWAttributeSet::OnRep_TotalMitigatedDamage(const FGameplayAttributeData& OldTotalMitigatedDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOWAttributeSet, TotalMitigatedDamage, OldTotalMitigatedDamage);
}

void UOWAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const
{
	// Source = Causer of the Effect
	// Target = Target of the Effect, Owner of the Attribute Set 

	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext(); 
	EffectProperties.EffectSpec = &Data.EffectSpec;
	EffectProperties.Magnitude = Data.EvaluatedData.Magnitude; 
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
				EffectProperties.SourcePlayerState = Pawn->GetPlayerState(); 
			}
		}
		if (EffectProperties.SourceController != nullptr)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn()); 
			EffectProperties.SourcePlayerState = EffectProperties.SourceController->PlayerState; 
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get(); 
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get(); 
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor); 
		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor); 
		if (EffectProperties.TargetController)
		{
			EffectProperties.TargetPlayerState = EffectProperties.TargetController->PlayerState;
		}
		else if (APawn* Pawn = Cast<APawn>(EffectProperties.TargetAvatarActor))
		{
			EffectProperties.TargetPlayerState = Pawn->GetPlayerState();
		}
	}
}

void UOWAttributeSet::HandleIncomingDamage(const FEffectProperties& EffectProperties)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f); 

	// Process Positive Incoming Damage 
	if (LocalIncomingDamage > 0.f)
	{
		// Damage Shield, Armor, and Health in that order.
		float RemainingDamage = LocalIncomingDamage; 
		// Should Damage Shield
		bool ShouldDamageShield = (GetShield() > 0.f) && (RemainingDamage > 0.f); 
		if (ShouldDamageShield)
		{
			float ShieldDamage = FMath::Min(RemainingDamage, GetShield()); 
			SetShield(GetShield() - ShieldDamage); 
			RemainingDamage -= ShieldDamage; 
		}
		// Should Damage Armor
		bool ShouldDamageArmor = (GetArmor() > 0.f) && (RemainingDamage > 0.f); 
		if (ShouldDamageArmor)
		{
			// TODO - if Attack Type is Laser or Not,
			// Using Effect Properties and Make OWAbilitySystemLibrary Getter Function 
			if (RemainingDamage >= 10.f)
			{
				RemainingDamage -= 5.f; 
			}
			else
			{
				RemainingDamage /= 2.f; 
			}
			float ArmorDamage = FMath::Min(RemainingDamage, GetArmor()); 
			SetArmor(GetArmor() - ArmorDamage); 
			RemainingDamage -= ArmorDamage; 
		}
		// Should Damage Health 
		bool ShouldDamageHealth = (GetHealth() > 0.f) && (RemainingDamage > 0.f); 
		if (ShouldDamageHealth)
		{
			float NewHealth = GetHealth() - RemainingDamage; 
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth())); 
		}

		// Check if Actor related to Attribute Set is Fatal 
		const bool bFatal = GetHealth() <= 0.f; 
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor); 
			if (CombatInterface)
			{
				CombatInterface->Die(UOWAbilitySystemLibrary::GetDeathImpulse(EffectProperties.EffectContextHandle));
			}
			SendHeroKilledEvent(EffectProperties);
			SendXPEvent(EffectProperties); 
		}
		else
		{
			// Try Activate Hit React Effect 
			if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(EffectProperties.TargetCharacter))
			{
				FGameplayTagContainer TagContainer; 
				TagContainer.AddTag(FOWGameplayTags::Get().Effects_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer); 
			}
			const FVector& KnockbackForce = UOWAbilitySystemLibrary::GetKnockbackForce(EffectProperties.EffectContextHandle); 
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				EffectProperties.TargetCharacter->LaunchCharacter(KnockbackForce, true, true); 
			}
			// Broadcast Damage Effect Causer, Hit Actor, and Damage 
			OnDamageReceived.Broadcast(EffectProperties.SourceAvatarActor, EffectProperties.TargetAvatarActor, LocalIncomingDamage);
		}

		const bool bCriticalHit = UOWAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle); 

		/*if (UOWAbilitySystemLibrary::IsSuccessfulDebuff(EffectProperties.EffectContextHandle))
		{
			Debuff(EffectProperties); 
		}*/
	}
	// Process Negative Incoming Damage as Healing 
	else if (LocalIncomingDamage < 0.f)
	{
		// Heal Health, Armor, and Shield in that order.
		float HealAmount = -LocalIncomingDamage;
		// Should Heal Health 
		bool ShouldHealHealth = GetHealth() < GetMaxHealth();
		if (ShouldHealHealth)
		{
			float HealthHeal = FMath::Min(HealAmount, GetMaxHealth() - GetHealth());
			SetHealth(GetHealth() + HealthHeal);
			HealAmount -= HealthHeal;
		}
		// Should Heal Armor
		bool ShouldHealArmor = (GetMaxArmor() > 0.f) && (HealAmount > 0.f) && (GetHealth() >= GetMaxHealth()) && (GetMaxArmor() > GetArmor());
		if (ShouldHealArmor)
		{
			float ArmorHeal = FMath::Min(HealAmount, GetMaxArmor() - GetArmor());
			SetArmor(GetArmor() + ArmorHeal);
			HealAmount -= ArmorHeal;
		}
		// Should Heal Shield
		bool ShouldHealShield = (GetMaxShield() > 0.f) && (HealAmount > 0.f)
			&& (GetHealth() >= GetMaxHealth()) && (GetArmor() >= GetMaxArmor()) && (GetMaxShield() >= GetShield());
		if (ShouldHealShield)
		{
			float ShieldHeal = FMath::Min(HealAmount, GetMaxShield() - GetShield());
			SetShield(GetShield() + ShieldHeal);
		}
	}

	// Get Gameplay Message Subsystem and Damage Message GameplayTag 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
	FGameplayTag HeroDamagedTag = GameplayTags.Gameplay_Message_HeroDamaged;

	// Initialize Hero Damaged Info 
	FHeroDamagedInfo HeroDamagedInfo;
	// HeroDamagedInfo.DamageTag = DamageTag;
	HeroDamagedInfo.SourcePlayerState = EffectProperties.SourcePlayerState;
	HeroDamagedInfo.TargetPlayerState = EffectProperties.TargetPlayerState;
	HeroDamagedInfo.Damage = LocalIncomingDamage;
	HeroDamagedInfo.DamageTimeSeconds = GetWorld()->GetTimeSeconds();

	// Broadcast Hero Debuffed Message Using Gameplay Message Subsystem 
	GameplayMessageSubsystem.BroadcastMessage(HeroDamagedTag, HeroDamagedInfo); }

void UOWAttributeSet::HandleIncomingXP(const FEffectProperties& EffectProperties)
{
	const float LocalIncomingXP = GetIncomingXP(); 
	SetIncomingXP(0.f); 

	// Source Character is the owner. 
	if (EffectProperties.SourceCharacter->Implements<ULevelUpInterface>() && EffectProperties.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProperties.SourceCharacter); 
		const int32 CurrentXP = ILevelUpInterface::Execute_GetXP(EffectProperties.SourceCharacter); 

		const int32 NewLevel = ILevelUpInterface::Execute_FindLevelForXP(EffectProperties.SourceCharacter, CurrentXP + LocalIncomingXP); 
		const int32 NumLevelUps = NewLevel - CurrentLevel; 
		if (NumLevelUps > 0)
		{
			ILevelUpInterface::Execute_AddToPlayerLevel(EffectProperties.SourceCharacter, NumLevelUps); 

			int32 AttributePointsReward = 0; 
			int32 SpellPointsReward = 0;

			for (int32 i = 0; i < NumLevelUps; ++i)
			{
				AttributePointsReward += ILevelUpInterface::Execute_GetAttributePointsReward(EffectProperties.SourceCharacter, CurrentLevel + i);
				SpellPointsReward += ILevelUpInterface::Execute_GetSpellPointsReward(EffectProperties.SourceCharacter, CurrentLevel + i); 
			}
			ILevelUpInterface::Execute_AddToAttributePoints(EffectProperties.SourceCharacter, AttributePointsReward); 
			ILevelUpInterface::Execute_AddToSpellPoints(EffectProperties.SourceCharacter, SpellPointsReward); 
			ILevelUpInterface::Execute_LevelUp(EffectProperties.SourceCharacter); 
		}
		ILevelUpInterface::Execute_AddToXP(EffectProperties.SourceCharacter, LocalIncomingXP); 
	}
}

void UOWAttributeSet::SendHeroKilledEvent(const FEffectProperties& EffectProperties)
{
	// Send Death Gameplay Event 
	AActor* EffectInstigator = EffectProperties.EffectContextHandle.Get()->GetOriginalInstigator(); 
	AActor* EffectCauser = EffectProperties.EffectContextHandle.Get()->GetEffectCauser(); 
	OnDeath.Broadcast(EffectInstigator, EffectCauser, EffectProperties.EffectSpec, EffectProperties.Magnitude);

	// Broadcast HeroKilled Message Using Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	FGameplayTag HeroKilledTag = FOWGameplayTags::Get().Gameplay_Message_HeroKilled;
	FHeroKilledInfo HeroKilledInfo = FHeroKilledInfo(EffectProperties.SourcePlayerState, EffectProperties.TargetPlayerState);
	GameplayMessageSubsystem.BroadcastMessage(HeroKilledTag, HeroKilledInfo);
}

void UOWAttributeSet::SendXPEvent(const FEffectProperties& EffectProperties)
{
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() && EffectProperties.TargetCharacter->Implements<UOmnicInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProperties.TargetCharacter); 
		const EOmnicClass TargetClass = IOmnicInterface::Execute_GetOmnicClass(EffectProperties.TargetCharacter); 
		const int32 XPReward = UOWAbilitySystemLibrary::GetXPRewardFromClassAndLevel(EffectProperties.TargetCharacter, TargetClass, TargetLevel); 

		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
		FGameplayEventData Payload; 
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP; 
		Payload.EventMagnitude = XPReward; 
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProperties.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload); 	
	}
}

void UOWAttributeSet::Debuff(const FEffectProperties& EffectProperties)
{
	// Check Effect Properties' Each Ability System Component and Character of Source and Target 
	check(EffectProperties.SourceASC && EffectProperties.TargetASC && EffectProperties.SourceCharacter); 

	// Get GameplayTag Singleton Container
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

	// Make Source Gameplay Effect Context 
	FGameplayEffectContextHandle EffectContextHandle = EffectProperties.SourceASC->MakeEffectContext(); 
	EffectContextHandle.AddSourceObject(EffectProperties.SourceCharacter); 

	// Set Damage Type, Debuff Tag, Damage, Duration, and Frequency
	const FGameplayTag DamageType = UOWAbilitySystemLibrary::GetDamageType(EffectProperties.EffectContextHandle); 
	const FGameplayTag DebuffTag = UOWAbilitySystemLibrary::GetDebuffTag(EffectProperties.EffectContextHandle); 
	const float DebuffDamage = UOWAbilitySystemLibrary::GetDebuffDamage(EffectProperties.EffectContextHandle); 
	const float DebuffDuration = UOWAbilitySystemLibrary::GetDebuffDuration(EffectProperties.EffectContextHandle); 
	const float DebuffFrequency = UOWAbilitySystemLibrary::GetDebuffFrequency(EffectProperties.EffectContextHandle); 

	// Make Temporary GameplayEffect Object
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString()); 
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName)); 
	checkf(Effect, TEXT("Failed to create UGameplayEffect %s in UOWAttributeSet::Debuff()"), *DebuffName);

	// Set GameplayEffect's Duration Policy, Period, and Duration Magnitude 
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration; 
	Effect->Period = DebuffFrequency; 
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration); 
	
	FInheritedTagContainer TagContainer = FInheritedTagContainer(); 
	UTargetTagsGameplayEffectComponent& TargetTagsGameplayEffectComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	TagContainer.Added.AddTag(DebuffTag); 
	TagContainer.CombinedTags.AddTag(DebuffTag); 

	// If Debuff is Stun or ForcedMovement, Block Input Held, Pressed, and Released
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun) || DebuffTag.MatchesTagExact(GameplayTags.Debuff_ForcedMovement))
	{
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld); 
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputHeld);

		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputPressed);

		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges(TagContainer);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource; 
	Effect->StackLimitCount = 1; 

	// Add new Modifier to Modifiers' Last Index
	int32 Index = Effect->Modifiers.Num(); 
	Effect->Modifiers.Add(FGameplayModifierInfo()); 
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index]; 

	// Add Debuff Damage to Incoming Damage 
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage); 
	ModifierInfo.ModifierOp = EGameplayModOp::Additive; 
	ModifierInfo.Attribute = GetIncomingDamageAttribute(); 

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContextHandle, 1.f))
	{
		// Cast to Custom Gameplay Effect Context
		FOWGameplayEffectContext* OWGameplayEffectContext = static_cast<FOWGameplayEffectContext*>(MutableSpec->GetContext().Get()); 
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType)); 
		// Set Damage Type of Custom Gameplay Effect Context 
		OWGameplayEffectContext->SetDamageType(DebuffDamageType); 

		EffectProperties.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec); 
	}

	// Get Gameplay Message Subsystem and Debuff Message GameplayTag 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	FGameplayTag HeroDebuffedTag = GameplayTags.Gameplay_Message_HeroDebuffed; 

	// Initialize Hero Debuffed Info 
	FHeroDebuffedInfo HeroDebuffedInfo; 
	HeroDebuffedInfo.DebuffTag = DebuffTag; 
	HeroDebuffedInfo.SourcePlayerState = EffectProperties.SourcePlayerState; 
	HeroDebuffedInfo.TargetPlayerState = EffectProperties.TargetPlayerState; 
	HeroDebuffedInfo.EffectContextHandle = EffectContextHandle;
	HeroDebuffedInfo.DebuffDamage = DebuffDamage;
	HeroDebuffedInfo.DebuffDuration = DebuffDuration;
	HeroDebuffedInfo.DebuffFrequency = DebuffFrequency;
	HeroDebuffedInfo.DebuffTimeSeconds = GetWorld()->GetTimeSeconds(); 

	// Broadcast Hero Debuffed Message Using Gameplay Message Subsystem 
	GameplayMessageSubsystem.BroadcastMessage(HeroDebuffedTag, HeroDebuffedInfo);
}
