// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/**
 * OW Gameplay Tags
 * 
 * Singleton containing native Gameplay Tags 
 */
struct FOWGameplayTags
{
public:
	static const FOWGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags(); 

	/* Defense Attributes */
	FGameplayTag Attributes_Defense_Health; 
	FGameplayTag Attributes_Defense_MaxHealth;
	FGameplayTag Attributes_Defense_Armor;
	FGameplayTag Attributes_Defense_MaxArmor;
	FGameplayTag Attributes_Defense_TempArmor;
	FGameplayTag Attributes_Defense_Shield;
	FGameplayTag Attributes_Defense_MaxShield;
	FGameplayTag Attributes_Defense_TempShield;
	FGameplayTag Attributes_Defense_OverHealth;

	/* Secondary Attributes */
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_ShieldRegeneration; 

	/* Damage Attributes */
	FGameplayTag Damage; 
	FGameplayTag Damage_Physical; 
	FGameplayTag Damage_Laser; 
	FGameplayTag Damage_Stun; 
	FGameplayTag Damage_Airborne; 
	FGameplayTag Damage_ForcedMovement; 
	
	/* Resistance Attributes */
	FGameplayTag Attributes_Resistance_Laser; 

	/* Input Attributes - Move */
	FGameplayTag InputTag_Move_Forward;
	FGameplayTag InputTag_Move_Left; 
	FGameplayTag InputTag_Move_Backward;
	FGameplayTag InputTag_Move_Right; 
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_Jump; 

	/* Input Attributes - Weapon & Skill */
	FGameplayTag InputTag_Skill_1; 
	FGameplayTag InputTag_Skill_2; 
	FGameplayTag InputTag_Skill_3; 
	FGameplayTag InputTag_Ultimate; 
	FGameplayTag InputTag_QuickAttack; 
	FGameplayTag InputTag_Reload; 
	FGameplayTag InputTag_LMB; 
	FGameplayTag InputTag_RMB; 

	/* Player Input Attributes */
	FGameplayTag Player_Block_InputPressed; 
	FGameplayTag Player_Block_InputHeld; 
	FGameplayTag Player_Block_InputReleased; 

	/* Skill Attributes */
	FGameplayTag Attributes_Skill_SkillGauge; 
	FGameplayTag Attributes_Skill_UltimateGauge; 
	FGameplayTag Attributes_Skill_MaxUltimateGauge; 
	FGameplayTag Attributes_Skill_NumCurrentBullets; 
	FGameplayTag Attributes_Skill_NumMaxBullets; 

	/* Meta Types */
	FGameplayTag Attributes_Meta_IncomingXP;

	/* Cooldown Types */
	FGameplayTag Cooldown_Illiari_Outburst;
	FGameplayTag Cooldown_Illiari_HealingPylon;
	FGameplayTag Cooldown_Illiari_SolarRifle; 
	FGameplayTag Cooldown_Illiari_HealingRay; 

	FGameplayTag Cooldown_Reinhardt_RocketHammer; 
	FGameplayTag Cooldown_Reinhardt_BarrierField; 
	FGameplayTag Cooldown_Reinhardt_Charge; 
	FGameplayTag Cooldown_Reinhardt_FireStrike; 

	/* Cost Types */
	FGameplayTag Cost_Illiari_SolarRifle; 
	FGameplayTag Cost_Illiari_HealingRay; 

	/* Hit React */
	FGameplayTag Abilities_HitReact;
	FGameplayTag Effects_HitReact; 

	/* Ability Status Types */
	FGameplayTag Abilities_Status_Locked; 
	FGameplayTag Abilities_Status_Eligible; 
	FGameplayTag Abilities_Status_Unlocked; 
	FGameplayTag Abilities_Status_Equipped; 

	/* Debuff Types */
	FGameplayTag Debuff_Chance; 
	FGameplayTag Debuff_Damage; 
	FGameplayTag Debuff_Duration; 
	FGameplayTag Debuff_Frequency;

	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Airborne;
	FGameplayTag Debuff_ForcedMovement;

	/* Map of Damage Types to Debuffs */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs; 

	/* Hero GameplayTags - Illiari */
	FGameplayTag Abilities_Illiari_Outburst; 
	FGameplayTag Abilities_Illiari_HealingPylon; 
	FGameplayTag Abilities_Illiari_DemolishPylon; 
	FGameplayTag Abilities_Illiari_CaptiveSun; 
	FGameplayTag Abilities_Illiari_SolarRifle; 
	FGameplayTag Abilities_Illiari_HealingRay; 

	FGameplayTag GameplayCue_Illiari_HealingRay;

	/* Hero GameplayTags - Reinhardt */
	FGameplayTag Abilities_Reinhardt_Charge; 
	FGameplayTag Abilities_Reinhardt_FireStrike;
	FGameplayTag Abilities_Reinhardt_EarthShatter;
	FGameplayTag Abilities_Reinhardt_BarrierField;
	FGameplayTag Abilities_Reinhardt_RocketHammer; 

private:
	static FOWGameplayTags GameplayTags; 
};