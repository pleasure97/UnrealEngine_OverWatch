// Fill out your copyright notice in the Description page of Project Settings.


#include "OWGameplayTags.h"
#include "GameplayTagsManager.h"

FOWGameplayTags FOWGameplayTags::GameplayTags;

void FOWGameplayTags::InitializeNativeGameplayTags()
{
	/* Defense Attributes */

	GameplayTags.Attributes_Defense_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.Health"),
		FString("Health"));

	GameplayTags.Attributes_Defense_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.MaxHealth"),
		FString("Max Health"));

	GameplayTags.Attributes_Defense_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.Armor"),
		FString("Armor"));

	GameplayTags.Attributes_Defense_MaxArmor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.MaxArmor"),
		FString("Max Armor"));

	GameplayTags.Attributes_Defense_TempArmor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.TempArmor"),
		FString("Temp Armor"));

	GameplayTags.Attributes_Defense_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.Shield"),
		FString("Shield"));

	GameplayTags.Attributes_Defense_MaxShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.MaxShield"),
		FString("Max Shield"));

	GameplayTags.Attributes_Defense_TempShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.TempShield"),
		FString("Temp Shield"));

	GameplayTags.Attributes_Defense_OverHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense.OverHealth"),
		FString("Over Health"));

	/* Skill Attributes */
	GameplayTags.Attributes_Skill_SkillGauge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.SkillGauge"),
		FString("Skill Gauge"));

	GameplayTags.Attributes_Skill_UltimateGauge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.UltimateGauge"),
		FString("Ultimate Gauge"));

	/* Input Attributes - Move */

	GameplayTags.InputTag_Move_Forward = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Move.Forward"),
		FString("InputTag of Move Forward"));

	GameplayTags.InputTag_Move_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Move.Left"),
		FString("InputTag of Move Left"));

	GameplayTags.InputTag_Move_Backward = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Move.Backward"),
		FString("InputTag of Move Backward"));

	GameplayTags.InputTag_Move_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Move.Right"),
		FString("InputTag of Move Right"));

	GameplayTags.InputTag_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Crouch"),
		FString("InputTag of Crouch"));

	GameplayTags.InputTag_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Jump"),
		FString("InputTag of Jump"));

	/* Input Attributes - Weapon & Skill */

	GameplayTags.InputTag_Skill_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Skill.1"),
		FString("InputTag of Skill 1"));

	GameplayTags.InputTag_Skill_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Skill.2"),
		FString("InputTag of Skill 2"));

	GameplayTags.InputTag_Skill_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Skill.3"),
		FString("InputTag of Skill 3"));

	GameplayTags.InputTag_Ultimate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Ultimate"),
		FString("InputTag of Ultimate"));

	GameplayTags.InputTag_QuickAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.QuickAttack"),
		FString("InputTag of QuickAttack"));

	GameplayTags.InputTag_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Reload"),
		FString("InputTag of Reload"));

	GameplayTags.InputTag_DefaultFire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.DefaultFire"),
		FString("InputTag of DefaultFire"));

	GameplayTags.InputTag_SecondaryFire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SecondaryFire"),
		FString("InputTag of SecondaryFire"));

	/* Player Input Attributes */

	GameplayTags.Player_Input_Pressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Input.Pressed"),
		FString("Player Input Pressed")); 

	GameplayTags.Player_Input_Held = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Input.Held"),
		FString("Player Input Held"));

	GameplayTags.Player_Input_Released = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Input.Released"),
		FString("Player Input Released"));

	/* Skill Attributes */
	GameplayTags.Attributes_Skill_SkillGauge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.SkillGauge"),
		FString("Skill Gauge")); 

	/* Status Types */
	GameplayTags.Status_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Knockback"),
		FString("Knockback"));

	GameplayTags.Status_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Stun"),
		FString("Stun"));

	GameplayTags.Status_Airborne = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Airborne"),
		FString("Airborne"));

	GameplayTags.Status_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.HitReact"),
		FString("HitReact"));

	/* Cooldown Types */
	GameplayTags.Cooldown_Illiari_Outburst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Illiari.Outburst"),
		FString("Outburst"));

	GameplayTags.Cooldown_Illiari_HealingPylon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Illiari.HealingPylon"),
		FString("HealingPylon"));

	/* Hero GameplayTags - Illiari */

	GameplayTags.Abilities_Illiari_Outburst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.Outburst"),
		FString("Outburst Ability of Illiari"));

	GameplayTags.Abilities_Illiari_HealingPylon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.HealingPylon"),
		FString("Healing Pylon Ability of Illiari"));

	GameplayTags.Abilities_Illiari_DemolishPylon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.Outburst"),
		FString("Demolishing Pylon Ability of Illiari"));

	GameplayTags.Abilities_Illiari_CaptiveSun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.CaptiveSun"),
		FString("CaptiveSun Ability of Illiari"));
}
