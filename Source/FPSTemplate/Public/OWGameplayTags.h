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

	/* Player Input Attributes */
	FGameplayTag Player_Input_Pressed; 
	FGameplayTag Player_Input_Held; 
	FGameplayTag Player_Input_Released; 

private:
	static FOWGameplayTags GameplayTags; 
};