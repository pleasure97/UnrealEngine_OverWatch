// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerSkills.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "OWGameplayTags.h"
#include "Components/Border.h"
#include "UI/Widget/PlayerSkill.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystemComponent.h"

void UPlayerSkills::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	TagsToBorders.Add(GameplayTags.InputTag_Skill_1, Border_LShift); 
	TagsToBorders.Add(GameplayTags.InputTag_Skill_2, Border_E); 
	TagsToBorders.Add(GameplayTags.InputTag_Skill_3, Border_F);
	TagsToBorders.Add(GameplayTags.InputTag_RMB, Border_RMB); 
}

void UPlayerSkills::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UPlayerSkills::InitializePlayerSkill); 
	}
}

void UPlayerSkills::InitializePlayerSkill(const FOWAbilityInfo& Info)
{
	if (!TagsToBorders.Contains(Info.InputTag) || !Info.Icon) return; 

	UBorder* Border = TagsToBorders[Info.InputTag]; 
	Border->SetVisibility(ESlateVisibility::Visible); 

	BlockAbilitiesWithTags.Add(Info.AbilityTag, Info.BlockAbilitiesWithTag); 

	UPlayerSkill* PlayerSkill = CreateWidget<UPlayerSkill>(this, PlayerSkillClass);
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		PlayerSkill->SetWidgetController(OverlayWidgetController);

		if (Info.AbilityTag.IsValid())
		{
			OverlayWidgetController->AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &UPlayerSkills::OnAbilityActivated); 
			OverlayWidgetController->AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UPlayerSkills::OnAbilityEnded); 

			PlayerSkill->SetWidgetInfo(Info);
			PlayerSkill->SetCooldownInfo(Info);

			Border->AddChild(PlayerSkill);
			TagsToSkills.Add(Info.AbilityTag, PlayerSkill);
		}
	}
}

void UPlayerSkills::OnAbilityActivated(UGameplayAbility* ActivatedAbility)
{
	if (!ActivatedAbility) return; 

	const FGameplayTagContainer& ActivatedAbilityTags = ActivatedAbility->AbilityTags;

	for (const FGameplayTag& ActivatedAbilityTag : ActivatedAbilityTags)
	{
		for (const auto& TagToSkill : TagsToSkills)
		{
			const FGameplayTagContainer& BlockAbilitiesTags = BlockAbilitiesWithTags[ActivatedAbilityTag]; 
			for (const FGameplayTag& BlockAbilityTag : BlockAbilitiesTags)
			{
				UPlayerSkill* PlayerSkill = TagsToSkills[BlockAbilityTag]; 
				PlayerSkill->UpdateBlockedByTag(true); 
			}
		}
	}	
}

void UPlayerSkills::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	const FGameplayTagContainer& EndedAbilityTags = AbilityEndedData.AbilityThatEnded->AbilityTags;

	for (const FGameplayTag& EndedAbilityTag : EndedAbilityTags)
	{
		for (const auto& TagToSkill : TagsToSkills)
		{
			const FGameplayTagContainer& BlockAbilitiesTags = BlockAbilitiesWithTags[EndedAbilityTag];
			for (const FGameplayTag& BlockAbilityTag : BlockAbilitiesTags)
			{
				UPlayerSkill* PlayerSkill = TagsToSkills[BlockAbilityTag];
				PlayerSkill->UpdateBlockedByTag(false);
			}
		}
	}
}
