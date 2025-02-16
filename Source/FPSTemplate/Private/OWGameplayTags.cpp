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

	GameplayTags.Attributes_Skill_NumCurrentBullets = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.NumCurrentBullets"),
		FString("The Number of Current Bullets"));

	GameplayTags.Attributes_Skill_NumMaxBullets = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.NumMaxBullets"),
		FString("The Number of Maximum Bullets"));

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

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("InputTag of Left Mouse Button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("InputTag of Right Mouse Button"));

	/* Player Input Attributes */

	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Player Input Pressed Blocked")); 

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Player Input Held Blocked"));

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Player Input Released Blocked"));

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

	/* Cooldown Types - Illiari */
	GameplayTags.Cooldown_Illiari_Outburst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Illiari.Outburst"),
		FString("Cooldown of Outburst"));

	GameplayTags.Cooldown_Illiari_HealingPylon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Illiari.HealingPylon"),
		FString("Cooldown of HealingPylon"));

	GameplayTags.Cooldown_Illiari_SolarRifle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Illiari.SolarRifle"),
		FString("Cooldown of SolarRifle"));

	GameplayTags.Cooldown_Illiari_HealingRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Illiari.HealingRay"),
		FString("Cooldown of HealingRay"));

	/* Cooldown Types - Reinhardt */

	GameplayTags.Cooldown_Reinhardt_RocketHammer = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Reinhardt.RocketHammer"),
		FString("Cooldown of RocketHammer"));

	GameplayTags.Cooldown_Reinhardt_BarrierField = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Reinhardt.BarrierField"),
		FString("Cooldown of BarrierField"));

	GameplayTags.Cooldown_Reinhardt_Charge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Reinhardt.Charge"),
		FString("Cooldown of Charge"));

	GameplayTags.Cooldown_Reinhardt_FireStrike = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Reinhardt.FireStrike"),
		FString("Cooldown of FireStrike"));

	/* Cost Types */
	GameplayTags.Cost_Illiari_SolarRifle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cost.Illiari.SolarRifle"),
		FString("Cost of SolarRifle"));

	GameplayTags.Cost_Illiari_HealingRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cost.Illiari.HealingRay"),
		FString("Cost of HealingRay"));
	
	/* Ability Status Types */
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status"));

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status"));

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status"));

	/* Debuff Types */
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff Stun"));

	/* Hero GameplayTags - Illiari */

	GameplayTags.Abilities_Illiari_Outburst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.Outburst"),
		FString("Illiari's Outburst Ability"));

	GameplayTags.Abilities_Illiari_HealingPylon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.HealingPylon"),
		FString("Illiari's Healing Pylon Ability"));

	GameplayTags.Abilities_Illiari_DemolishPylon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.DemolishPylon"),
		FString("Illiari's Demolishing Pylon Ability"));

	GameplayTags.Abilities_Illiari_CaptiveSun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.CaptiveSun"),
		FString("Illiari's CaptiveSun Ability"));

	GameplayTags.Abilities_Illiari_SolarRifle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.SolarRifle"),
		FString("Illiari's SolarRifle Ability"));

	GameplayTags.Abilities_Illiari_HealingRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Illiari.HealingRay"),
		FString("Illiari's HealingRay Ability"));

	/* Hero GameplayTags - Reinhardt */
	GameplayTags.Abilities_Reinhardt_Charge= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reinhardt.Charge"),
		FString("Reinhardt's Charge Ability"));

	GameplayTags.Abilities_Reinhardt_FireStrike = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reinhardt.FireStrike"),
		FString("Reinhardt's Fire Strike Ability"));

	GameplayTags.Abilities_Reinhardt_EarthShatter = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reinhardt.EarthShatter"),
		FString("Reinhardt's EarthShatter Ability"));

	GameplayTags.Abilities_Reinhardt_BarrierField= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reinhardt.BarrierField"),
		FString("Reinhardt's BarrierField Ability"));

	GameplayTags.Abilities_Reinhardt_RocketHammer = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reinhardt.RocketHammer"),
		FString("Reinhardt's RocketHammer Ability"));

}
