// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerSkills.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "OWGameplayTags.h"
#include "Components/Border.h"
#include "UI/Widget/PlayerSkill.h"
#include "AbilitySystem/Data/HeroInfo.h"

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
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UPlayerSkills::SetChildToBorder); 
	}
}

void UPlayerSkills::SetChildToBorder(const FOWAbilityInfo& Info)
{
	if (!TagsToBorders.Contains(Info.InputTag) || !Info.Icon) return; 

	UBorder* Border = TagsToBorders[Info.InputTag]; 
	Border->SetVisibility(ESlateVisibility::Visible); 

	UPlayerSkill* PlayerSkill = CreateWidget<UPlayerSkill>(this, PlayerSkillClass);
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		PlayerSkill->SetWidgetController(OverlayWidgetController);
	}
	PlayerSkill->SetWidgetInfo(Info);
	PlayerSkill->SetCooldownInfo(Info); 

	Border->AddChild(PlayerSkill); 
}
