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

	/* Secondary Attributes */

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Critical Hit Damage"));

	GameplayTags.Attributes_Secondary_ShieldRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ShieldRegeneration"),
		FString("Shield Regeneration"));

	/* Damage Attributes */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")); 

	GameplayTags.Damage_HitScan = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.HitScan"),
		FString("HitScan Damage"));

	GameplayTags.Damage_Projectile = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Projectile"),
		FString("Projectile Damage"));

	GameplayTags.Damage_Laser = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Laser"),
		FString("Laser Damage"));

	GameplayTags.Damage_MeleeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.MeleeAttack"),
		FString("Melee Attack Damage"));

	GameplayTags.Damage_Healing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Healing"),
		FString("Healing is regarded as dealing Negative Damage."));

	GameplayTags.Damage_Immune = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Immune"),
		FString("Make Incoming Damage zero."));

	GameplayTags.DamageTypes.Add(GameplayTags.Damage_HitScan);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Projectile);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Laser);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_MeleeAttack);

	/* Resistance Attributes */

	GameplayTags.Attributes_Resistance_Laser = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Laser"),
		FString("Resistance of Laser Damage"));

	/* Skill Attributes */

	GameplayTags.Attributes_Skill_SkillGauge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.SkillGauge"),
		FString("Skill Gauge"));

	GameplayTags.Attributes_Skill_UltimateGauge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.UltimateGauge"),
		FString("Ultimate Gauge"));

	GameplayTags.Attributes_Skill_MaxUltimateGauge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Skill.MaxUltimateGauge"),
		FString("Max Ultimate Gauge"));

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

	GameplayTags.InputTag_ScoreBoard = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ScoreBoard"),
		FString("InputTag of ScoreBoard Button"));

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

	/* Meta Attributes */
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP")); 

	/* Match Attributes */
	GameplayTags.Attributes_Match_NumKills = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Match.NumKills"),
		FString("Num Kills")); 

	GameplayTags.Attributes_Match_NumDeaths = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Match.NumDeaths"),
		FString("Num Deaths"));

	GameplayTags.Attributes_Match_NumAssists = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Match.NumAssists"),
		FString("Num Assists"));

	GameplayTags.Attributes_Match_TotalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Match.TotalDamage"),
		FString("Total Damage"));

	GameplayTags.Attributes_Match_TotalHeal = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Match.TotalHeal"),
		FString("Total Heal"));

	GameplayTags.Attributes_Match_TotalMitigatedDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Match.TotalMitigatedDamage"),
		FString("Total Mitigated Damage"));

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

	/* Common Abilities */
	GameplayTags.Abilities_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reload"),
		FString("Reload Ability"));

	GameplayTags.Abilities_IgnoreDeath= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.IgnoreDeath"),
		FString("Ability which ignores Death"));

	/* Hit React */
	GameplayTags.Abilities_HitReact= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability"));

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Hit React Effects"));

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

	GameplayTags.Abilities_Status_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Death"),
		FString("Death Status"));

	/* Debuff Types */
	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance"));

	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage"));

	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency"));

	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration"));

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff Stun"));

	GameplayTags.Debuff_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Knockback"),
		FString("Debuff Knockback"));

	GameplayTags.Debuff_ForcedMovement = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.ForcedMovement"),
		FString("Debuff ForcedMovement"));

	GameplayTags.Debuff_Laceration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Laceration"),
		FString("Debuff Laceration"));

	/* Gameplay Cues */
	GameplayTags.GameplayCue_HeroSelection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.HeroSelection"),
		FString("GameplayCue When Hero Selection Phase is Active")); 

	GameplayTags.GameplayCue_SwitchInning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.SwitchInning"),
		FString("GameplayCue When Switching Inning")); 

	GameplayTags.GameplayCue_MatchDecided = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.MatchDecided"),
		FString("GameplayCue When Match is Decided"));

	GameplayTags.GameplayCue_DamageImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.DamageImpact"),
		FString("GameplayCue When Damaged"));

	GameplayTags.GameplayCue_HealImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.HealImpact"),
		FString("GameplayCue When Healed"));

	GameplayTags.GameplayCue_Illiari_HealingRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Illiari.HealingRay"),
		FString("Illiari's Healing Ray GameplayCue"));

	GameplayTags.GameplayCue_Illiari_Outburst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Illiari.Outburst"),
		FString("Illiari's Outburst GameplayCue"));

	GameplayTags.GameplayCue_Illiari_Laceration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Illiari.Laceration"),
		FString("Illiari's Laceration GameplayCue"));

	/* Gameplay Message */

	GameplayTags.Gameplay_Message_HealthPlate_Add = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.HealthPlate.Add"),
		FString("Gameplay Message for Adding HealthPlate"));

	GameplayTags.Gameplay_Message_HealthPlate_Remove = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.HealthPlate.Remove"),
		FString("Gameplay Message for Removing HealthPlate"));

	GameplayTags.Gameplay_Message_HealthPlate_Discover = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.HealthPlate.Discover"),
		FString("Gameplay Message for Discovering HealthPlate")); 

	GameplayTags.Gameplay_Message_HeroDamaged = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.HeroDamaged"),
		FString("Gameplay Message for Event Hero Damaged"));

	GameplayTags.Gameplay_Message_HeroDebuffed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.HeroDebuffed"),
		FString("Gameplay Message for Event Hero Debuffed"));

	GameplayTags.Gameplay_Message_HeroKilled = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.HeroKilled"),
		FString("Gameplay Message for Event Hero Killed")); 

	GameplayTags.Gameplay_Message_KillFeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.KillFeed"),
		FString("Gameplay Message for Updating Kill Feed"));

	GameplayTags.Gameplay_Message_Respawn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.Respawn"),
		FString("Gameplay Message for Event Hero Respawning"));

	GameplayTags.Gameplay_Message_MatchBeginCountdown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.MatchBeginCountdown"),
		FString("Gameplay Message for Match Begin Countdown"));

	GameplayTags.Gameplay_Message_AssaultProgress = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Gameplay.Message.AssaultProgress"),
		FString("Gameplay Message for Assault Progress"));

	/* Game Phase */
	GameplayTags.GamePhase_HeroSelection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.HeroSelection"),
		FString("Game Phase When Selecting a Hero"));

	GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.MatchPreparation.FirstTeamOffense"),
		FString("Game Phase When the First Team Prepares to Attack"));

	GameplayTags.GamePhase_MatchPreparation_SecondTeamOffense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.MatchPreparation.SecondTeamOffense"),
		FString("Game Phase When the Second Team Prepares to Attack"));

	GameplayTags.GamePhase_SwitchInning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.SwitchInning"),
		FString("Game Phase When Switching Offense and Defense"));

	GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.MatchInProgress.FirstTeamOffense"),
		FString("Game Phase When the First Team Attacks"));

	GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.MatchInProgress.SecondTeamOffense"),
		FString("Game Phase When the Second Team Attacks"));

	GameplayTags.GamePhase_PostMatch = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GamePhase.PostMatch"),
		FString("Game Phase After Match"));

	/* Battle Field */
	GameplayTags.BattleField_Assault_TeamScore = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("BattleField.Assault.TeamScore"),
		FString("Team Score in Assault Battle Field"));

	/* Set By Caller */

	GameplayTags.Data_Cooldown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.Cooldown"),
		FString("Gamplay Tag When Cooldown is Set By Caller"));

	GameplayTags.Data_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.CriticalHitDamage"),
		FString("Gamplay Tag When Critical Hit Damage is Set By Caller"));

	/* Animation */
	GameplayTags.Event_Movement_AimDownSight = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Movement.AimDownSight"),
		FString("Aim Down Sight Event Tag"));

	GameplayTags.Event_Movement_WeaponFire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Movement.WeaponFire"),
		FString("Weapon Fire Event Tag"));

	GameplayTags.Event_Movement_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Movement.Reload"),
		FString("Reload Event Tag"));

	/* Event */
	GameplayTags.Event_ChangeHero = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.ChangeHero"),
		FString("Change Hero Event Tag"));

	GameplayTags.Event_ReloadDone = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.ReloadDone"),
		FString("Reload Done Event Tag"));

	GameplayTags.Event_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Death"),
		FString("Death Event Tag"));

	GameplayTags.Event_ResetHero = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.ResetHero"),
		FString("Reset Hero Tag"));

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

	GameplayTags.Event_Montage_Outburst = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.Outburst"),
		FString("Illiari's Outburst Montage Event Tag"));

	GameplayTags.Event_Montage_HealingRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.HealingRay"),
		FString("Illiari's Healing Ray Montage Event Tag"));

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

	GameplayTags.Event_Montage_BarrierField= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.BarrierField"),
		FString("Reinhardt's BarrierField Montage Event Tag"));

}
