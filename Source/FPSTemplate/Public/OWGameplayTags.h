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
	FGameplayTag Attributes_Defense_TempArmor;
	FGameplayTag Attributes_Defense_Shield;
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
	FGameplayTag InputTag_Ultimate; 
	FGameplayTag InputTag_QuickAttack; 
	FGameplayTag InputTag_Reload; 
	FGameplayTag InputTag_DefaultFire; 
	FGameplayTag InputTag_SecondaryFire; 

	/* Player Input Attributes */
	FGameplayTag Player_Input_Pressed; 
	FGameplayTag Player_Input_Held; 
	FGameplayTag Player_Input_Released; 

	/* Skill Attributes */
	FGameplayTag Attributes_Skill_SkillGauge; 

private:
	static FOWGameplayTags GameplayTags; 
};