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
	FGameplayTag InputTag_DefaultFire; 
	FGameplayTag InputTag_SecondaryFire; 

	/* Player Input Attributes */
	FGameplayTag Player_Block_InputPressed; 
	FGameplayTag Player_Block_InputHeld; 
	FGameplayTag Player_Block_InputReleased; 

	/* Skill Attributes */
	FGameplayTag Attributes_Skill_SkillGauge; 
	FGameplayTag Attributes_Skill_UltimateGauge; 

	/* Status Types */
	FGameplayTag Status_Knockback; 
	FGameplayTag Status_Stun; 
	FGameplayTag Status_Airborne; 
	FGameplayTag Status_HitReact; 

	/* Cooldown Types */
	FGameplayTag Cooldown_Illiari_Outburst;
	FGameplayTag Cooldown_Illiari_HealingPylon;

	/* Ability Status Types */
	FGameplayTag Abilities_Status_Locked; 
	FGameplayTag Abilities_Status_Eligible; 
	FGameplayTag Abilities_Status_Unlocked; 
	FGameplayTag Abilities_Status_Equipped; 

	/* Hero GameplayTags - Illiari */
	FGameplayTag Abilities_Illiari_Outburst; 
	FGameplayTag Abilities_Illiari_HealingPylon; 
	FGameplayTag Abilities_Illiari_DemolishPylon; 
	FGameplayTag Abilities_Illiari_CaptiveSun; 

private:
	static FOWGameplayTags GameplayTags; 
};