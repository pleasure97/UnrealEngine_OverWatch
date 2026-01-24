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

	// Get GameplayTag Singleton Container
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	// Match Skill Input GameplayTags and Borders
	TagsToBorders.Add(GameplayTags.InputTag_Skill_1, Border_LShift); 
	TagsToBorders.Add(GameplayTags.InputTag_Skill_2, Border_E); 
	TagsToBorders.Add(GameplayTags.InputTag_Skill_3, Border_F);
	TagsToBorders.Add(GameplayTags.InputTag_RMB, Border_RMB); 
}

void UPlayerSkills::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	// Set Overlay Widget Controller and Bind Ability Info Delegate
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UPlayerSkills::InitializePlayerSkill); 
	}
}

void UPlayerSkills::InitializePlayerSkill(const FOWAbilityInfo& Info)
{
	if (!TagsToBorders.Contains(Info.InputTag) || !Info.Icon)
	{
		return;
	}

	UBorder* Border = TagsToBorders[Info.InputTag]; 
	if (IsValid(Border))
	{
		Border->SetVisibility(ESlateVisibility::Visible);
	}

	BlockAbilitiesWithTags.Add(Info.AbilityTag, Info.BlockAbilitiesWithTag); 

	UPlayerSkill* PlayerSkill = CreateWidget<UPlayerSkill>(this, PlayerSkillClass);
	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
	if (!IsValid(PlayerSkill) || !IsValid(OverlayWidgetController) || !Info.AbilityTag.IsValid())
	{
		return;
	}

	PlayerSkill->SetWidgetController(OverlayWidgetController);

	OverlayWidgetController->AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &UPlayerSkills::OnAbilityActivated);
	OverlayWidgetController->AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UPlayerSkills::OnAbilityEnded);

	PlayerSkill->SetWidgetInfo(Info);
	PlayerSkill->SetCooldownInfo(Info);

	Border->AddChild(PlayerSkill);
	TagsToSkills.Add(Info.AbilityTag, PlayerSkill);
}

void UPlayerSkills::OnAbilityActivated(UGameplayAbility* ActivatedAbility)
{
	if (!IsValid(ActivatedAbility))
	{
		return;
	}

	const FGameplayTagContainer& ActivatedAbilityTags = ActivatedAbility->AbilityTags;

	for (const FGameplayTag& ActivatedAbilityTag : ActivatedAbilityTags)
	{
		if (TagsToSkills.Find(ActivatedAbilityTag))
		{
			UPlayerSkill* ActivatedPlayerSkill = TagsToSkills[ActivatedAbilityTag];
			ActivatedPlayerSkill->UpdateActivatedByTag(true);
		}

		for (const auto& TagToSkill : TagsToSkills)
		{
			if (!BlockAbilitiesWithTags.Find(ActivatedAbilityTag))
			{
				return;
			}
			const FGameplayTagContainer& BlockAbilitiesTags = BlockAbilitiesWithTags[ActivatedAbilityTag]; 
			for (const FGameplayTag& BlockAbilityTag : BlockAbilitiesTags)
			{
				UPlayerSkill* BlockedPlayerSkill = TagsToSkills[BlockAbilityTag]; 
				if (IsValid(BlockedPlayerSkill))
				{
					BlockedPlayerSkill->UpdateBlockedByTag(true);
				}
			}
		}
	}	
}

void UPlayerSkills::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	for (const FGameplayTag& EndedTag : AbilityEndedData.AbilityThatEnded->AbilityTags)
	{
		if (TagsToSkills.Find(EndedTag))
		{
			UPlayerSkill* ActivatedPlayerSkill = TagsToSkills[EndedTag];
			ActivatedPlayerSkill->UpdateActivatedByTag(false);
		}
		
		const FGameplayTagContainer* BlockAbilitiesContainer = BlockAbilitiesWithTags.Find(EndedTag);
		if (!BlockAbilitiesContainer)
		{
			continue;
		}

		for (const FGameplayTag& BlockAbilityTag : BlockAbilitiesContainer->GetGameplayTagArray())
		{
			if (TObjectPtr<UPlayerSkill>* PlayerSkillPointer = TagsToSkills.Find(BlockAbilityTag))
			{
				if (UPlayerSkill* BlockedPlayerSkill = *PlayerSkillPointer)
				{
					BlockedPlayerSkill->UpdateBlockedByTag(false);
				}
			}
		}
	}
}
