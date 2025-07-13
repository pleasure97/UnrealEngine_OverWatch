// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelectionOverlay.h"
#include "Player/OWPlayerState.h"
#include "Components/TextBlock.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "UI/Widget/HeroSelection/HeroSelectionList.h"
#include "UI/Widget/HeroSelection/TeamMemberInfoList.h"

void UHeroSelectionOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Bind Hero Name Changed Delegate of Player State
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetOwningPlayerState()))
	{
		OWPlayerState->OnHeroNameChangedDelegate.AddUObject(this, &UHeroSelectionOverlay::OnHeroNameChanged);
	}

	// Setup for Initial UI Designs
	if (TextBlock_MissionDescription)
	{
		TextBlock_MissionDescription->SetVisibility(ESlateVisibility::Visible);
	}

	if (TextBlock_WaitingForBattle)
	{
		TextBlock_WaitingForBattle->SetVisibility(ESlateVisibility::Collapsed); 
	}

	if (TextBlock_HeroName)
	{
		TextBlock_HeroName->SetVisibility(ESlateVisibility::Collapsed); 
	}

	// Initialize Hero Selection List 
	if (WBP_HeroSelectionList)
	{
		WBP_HeroSelectionList->InitializeHeroSelectionList(); 
	}

	if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		int32 TeamID = TeamSubsystem->FindTeamFromObject(GetOwningPlayerState());

		// TODO - Use GamePhaseSubsystem::WhenPhaseStartsOrIsActive() 
		if (TeamID == 1)
		{
			if (TextBlock_MainMission)
			{
				TextBlock_MainMission->SetColorAndOpacity(RedColor); 
				TextBlock_MainMission->SetText(AttackMissionText); 
			}
		}
		// TODO - Use GamePhaseSubsystem::WhenPhaseStartsOrIsActive() 
		else if (TeamID == 2)
		{
			if (TextBlock_MainMission)
			{
				TextBlock_MainMission->SetColorAndOpacity(BlueColor);
				TextBlock_MainMission->SetText(DefendMissionText);
			}
		}
	}
}

void UHeroSelectionOverlay::NativeDestruct()
{
	// Remove Binding of Hero Name Changed Delegate 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetOwningPlayerState()))
	{
		OWPlayerState->OnHeroNameChangedDelegate.RemoveAll(this);
	}

	Super::NativeDestruct(); 
}

void UHeroSelectionOverlay::OnHeroNameChanged(AOWPlayerState* PlayerState, EHeroName HeroName)
{
	if (TextBlock_HeroName)
	{
		// Check if Hero Name is None 
		if (HeroName == EHeroName::None)
		{
			TextBlock_HeroName->SetVisibility(ESlateVisibility::Collapsed); 
		}
		else
		{
			UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this); 
			if (HeroInfo->HeroInformation.Find(HeroName))
			{
				TextBlock_HeroName->SetText(HeroInfo->HeroInformation[HeroName].HeroDisplayName); 
			}
		}
	}
}



