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
	FGameplayTag Damage_HitScan; 
	FGameplayTag Damage_Projectile; 
	FGameplayTag Damage_Laser; 
	FGameplayTag Damage_MeleeAttack;

	FGameplayTag Damage_Healing; 

	FGameplayTag Damage_Immune; 

	TArray<FGameplayTag> DamageTypes; 
	
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
	FGameplayTag InputTag_ScoreBoard; 

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

	/* Meta Attributes */
	FGameplayTag Attributes_Meta_IncomingXP;

	/* Match Attributes */
	FGameplayTag Attributes_Match_NumKills;
	FGameplayTag Attributes_Match_NumDeaths;
	FGameplayTag Attributes_Match_NumAssists;
	FGameplayTag Attributes_Match_TotalDamage;
	FGameplayTag Attributes_Match_TotalHeal;
	FGameplayTag Attributes_Match_TotalMitigatedDamage;

	/* Cost Types */
	FGameplayTag Cost_Illiari_SolarRifle; 
	FGameplayTag Cost_Illiari_HealingRay; 

	/* Common Abilities */
	FGameplayTag Abilities_Reload; 
	FGameplayTag Abilities_IgnoreDeath; 

	/* Hit React */
	FGameplayTag Abilities_HitReact;
	FGameplayTag Effects_HitReact; 

	/* Ability Status Types */
	FGameplayTag Abilities_Status_Locked; 
	FGameplayTag Abilities_Status_Eligible; 
	FGameplayTag Abilities_Status_Unlocked; 
	FGameplayTag Abilities_Status_Equipped; 
	FGameplayTag Abilities_Status_Death; 

	/* Debuff Types */
	FGameplayTag Debuff_Chance; 
	FGameplayTag Debuff_Damage; 
	FGameplayTag Debuff_Duration; 
	FGameplayTag Debuff_Frequency;

	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Knockback;
	FGameplayTag Debuff_ForcedMovement;
	FGameplayTag Debuff_Laceration; 

	/* Gameplay Cue */
	FGameplayTag GameplayCue_HeroSelection;
	FGameplayTag GameplayCue_HeroDeath; 
	FGameplayTag GameplayCue_SwitchInning; 
	FGameplayTag GameplayCue_MatchDecided; 

	FGameplayTag GameplayCue_DamageImpact; 
	FGameplayTag GameplayCue_HealImpact; 

	FGameplayTag GameplayCue_Illiari_HealingRay;
	FGameplayTag GameplayCue_Illiari_Outburst;
	FGameplayTag GameplayCue_Illiari_Laceration; 

	FGameplayTag GameplayCue_Tracer_PulsePistols; 
	FGameplayTag GameplayCue_Tracer_Blink; 
	FGameplayTag GameplayCue_Tracer_Recall; 
	FGameplayTag GameplayCue_Tracer_PulseBomb; 

	/* Gameplay Message */
	FGameplayTag Gameplay_Message_HealthPlate_Add; 
	FGameplayTag Gameplay_Message_HealthPlate_Remove; 
	FGameplayTag Gameplay_Message_HealthPlate_Discover; 
	FGameplayTag Gameplay_Message_HeroDamaged; 
	FGameplayTag Gameplay_Message_HeroDebuffed;
	FGameplayTag Gameplay_Message_HeroKilled; 
	FGameplayTag Gameplay_Message_KillFeed; 
	FGameplayTag Gameplay_Message_RespawnWaiting; 
	FGameplayTag Gameplay_Message_RespawnCompleted; 
	FGameplayTag Gameplay_Message_MatchBeginCountdown; 
	FGameplayTag Gameplay_Message_AssaultProgress; 
	FGameplayTag Gameplay_Message_MatchScoringComponent; 

	/* Game Phase */
	FGameplayTag GamePhase_HeroSelection_FirstHeroSelection; 
	FGameplayTag GamePhase_HeroSelection_SecondHeroSelection; 
	FGameplayTag GamePhase_MatchPreparation_FirstTeamOffense; 
	FGameplayTag GamePhase_MatchPreparation_SecondTeamOffense; 
	FGameplayTag GamePhase_SwitchInning;
	FGameplayTag GamePhase_MatchInProgress_FirstTeamOffense;
	FGameplayTag GamePhase_MatchInProgress_SecondTeamOffense;
	FGameplayTag GamePhase_PostMatch; 

	/* Battle Field */
	FGameplayTag BattleField_Assault_TeamScore; 

	/* Set By Caller */ 
	FGameplayTag Data_Cooldown; 
	FGameplayTag Data_CriticalHitDamage; 

	/* Animation */
	FGameplayTag Event_Movement_AimDownSight; 
	FGameplayTag Event_Movement_WeaponFire; 
	FGameplayTag Event_Movement_Reload; 

	/* Event */
	FGameplayTag Event_ChangeHero; 
	FGameplayTag Event_ReloadDone; 
	FGameplayTag Event_Death; 
	FGameplayTag Event_ResetHero; 
	FGameplayTag Event_RespawnHero; 

	/* Hero GameplayTags - Illiari */
	FGameplayTag Abilities_Illiari_Outburst; 
	FGameplayTag Abilities_Illiari_HealingPylon; 
	FGameplayTag Abilities_Illiari_DemolishPylon; 
	FGameplayTag Abilities_Illiari_CaptiveSun; 
	FGameplayTag Abilities_Illiari_SolarRifle; 
	FGameplayTag Abilities_Illiari_HealingRay; 

	FGameplayTag Cooldown_Illiari_Outburst;
	FGameplayTag Cooldown_Illiari_HealingPylon;
	FGameplayTag Cooldown_Illiari_SolarRifle;
	FGameplayTag Cooldown_Illiari_HealingRay;

	FGameplayTag Event_Montage_Outburst; 
	FGameplayTag Event_Montage_HealingRay; 

	/* Hero GameplayTags - Reinhardt */
	FGameplayTag Abilities_Reinhardt_Charge; 
	FGameplayTag Abilities_Reinhardt_FireStrike;
	FGameplayTag Abilities_Reinhardt_EarthShatter;
	FGameplayTag Abilities_Reinhardt_BarrierField;
	FGameplayTag Abilities_Reinhardt_RocketHammer; 

	FGameplayTag Cooldown_Reinhardt_RocketHammer;
	FGameplayTag Cooldown_Reinhardt_BarrierField;
	FGameplayTag Cooldown_Reinhardt_Charge;
	FGameplayTag Cooldown_Reinhardt_FireStrike;

	FGameplayTag Event_Montage_BarrierField;

	/* Hero GameplayTags - Tracer */
	FGameplayTag Abilities_Tracer_PulsePistols; 
	FGameplayTag Abilities_Tracer_Blink; 
	FGameplayTag Abilities_Tracer_Recall; 
	FGameplayTag Abilities_Tracer_PulseBomb; 

	FGameplayTag Cooldown_Tracer_Blink;
	FGameplayTag Cooldown_Tracer_Recall;

private:
	static FOWGameplayTags GameplayTags; 
};