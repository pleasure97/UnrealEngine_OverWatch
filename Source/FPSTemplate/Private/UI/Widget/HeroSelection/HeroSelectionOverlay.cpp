// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelectionOverlay.h"
#include "OWGameplayTags.h"
#include "Player/OWPlayerState.h"
#include "Components/TextBlock.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/MatchScoringComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "UI/Widget/HeroSelection/HeroSelectionList.h"
#include "UI/Widget/HeroSelection/TeamMemberInfoList.h"

void UHeroSelectionOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Bind Hero Name Changed Delegate of Player State
	if (WBP_HeroSelectionList)
	{
		WBP_HeroSelectionList->HeroSelectButtonDelegate.AddDynamic(this, &UHeroSelectionOverlay::OnHeroSelectButtonClicked); 
	}

	// Setup for Initial UI Designs
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

	if (UWorld* World = GetWorld())
	{
		if (UOWTeamSubsystem* TeamSubsystem = World->GetSubsystem<UOWTeamSubsystem>())
		{
			OwnerPlayerState = Cast<AOWPlayerState>(GetOwningPlayerState());
			if (OwnerPlayerState)
			{
				OwnerTeamID = TeamSubsystem->FindTeamFromObject(OwnerPlayerState);
				if (OwnerTeamID < 0)
				{
					OwnerPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UHeroSelectionOverlay::OnTeamChanged);
				}
			}
		}

		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
		// Register Gameplay Message Listener Handles - First Hero Selection Game Phase
		FirstHeroSelectionListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, this, &UHeroSelectionOverlay::HandleCountdownTime);
		// Register Gameplay Message Listener Handles - First Match Preparation Game Phase
		FirstMatchPreparationListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		// Register Gameplay Message Listener Handles - First Team Offense Game Phase 
		FirstTeamOffenseListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		// Register Gameplay Message Listener Handles - Second Hero Selection Game Phase
		SecondHeroSelectionListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_HeroSelection_SecondHeroSelection, this, &UHeroSelectionOverlay::HandleCountdownTime);
		// Register Gameplay Message Listener Handles - Second Match Preparation Game Phase
		SecondMatchPreparationListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_MatchPreparation_SecondTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		// Register Gameplay Message Listener Handles - Second Team Offense Game Phase
		SecondTeamOffenseListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		// Register Gameplay Message Listener Handles - Post Match Game Phase
		PostMatchListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
				GameplayTags.GamePhase_PostMatch, this, &UHeroSelectionOverlay::HandleCountdownTime);
	}
}

void UHeroSelectionOverlay::HandleCountdownTime(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	ProcessPhaseRemainingTime(Payload.Verb, Payload.Magnitude);
}

void UHeroSelectionOverlay::OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID)
{
	if ((NewTeamID == 1) || (NewTeamID == 2))
	{
		OwnerTeamID = NewTeamID;
	}
}

void UHeroSelectionOverlay::ProcessPhaseRemainingTime(const FGameplayTag& PhaseTag, const float RemainingTime)
{
	// Set New Current Game Phase Tag and Prepare to Update Mission Description
	if (!CurrentGamePhaseTag.MatchesTagExact(PhaseTag))
	{
		CurrentGamePhaseTag = PhaseTag;
		bMissionDescriptionUpdated = false; 
	}

	// Update Remaining Time in Any Game Phase 
	if (TextBlock_RemainingTime)
	{
		TextBlock_RemainingTime->SetText(ConvertTimeText(RemainingTime));
	}

	// Early return if Mission Description is Updated 
	if (bMissionDescriptionUpdated)
	{
		return;
	}
	
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	// Game Phase - First Hero Selection 
	if (CurrentGamePhaseTag == GameplayTags.GamePhase_HeroSelection_FirstHeroSelection)
	{
		if (TextBlock_MissionDescription)
		{
			TextBlock_MissionDescription->SetText(WaitingForTeamConstructionText);
		}

		if (OwnerTeamID == 1)
		{
			if (TextBlock_MainMission)
			{
				TextBlock_MainMission->SetText(AttackMissionText); 
				TextBlock_MainMission->SetColorAndOpacity(FSlateColor(RedColor)); 
			}
			
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_MainMission)
			{
				TextBlock_MainMission->SetText(DefendMissionText);
				TextBlock_MainMission->SetColorAndOpacity(FSlateColor(BlueColor));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2")); 
		}
	} 
	// Game Phase - First Match Preparation 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (TextBlock_MissionDescription)
			{
				TextBlock_MissionDescription->SetText(AttackPreparationMissionText); 
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_MissionDescription)
			{
				TextBlock_MissionDescription->SetText(DefendPreparationMissionText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2"));
		}
	}
	// Game Phase - First Team Offense
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense)
	{
		if (TextBlock_MissionDescription)
		{
			TextBlock_MissionDescription->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// Game Phase - Second Hero Selection
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_HeroSelection_SecondHeroSelection)
	{
		if (TextBlock_MissionDescription)
		{
			TextBlock_MissionDescription->SetText(WaitingForTeamConstructionText);
			TextBlock_MissionDescription->SetVisibility(ESlateVisibility::Visible); 
		}

		if (OwnerTeamID == 1)
		{
			if (TextBlock_MainMission)
			{
				TextBlock_MainMission->SetText(DefendMissionText);
				TextBlock_MainMission->SetColorAndOpacity(FSlateColor(BlueColor));
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_MainMission)
			{
				TextBlock_MainMission->SetText(AttackMissionText);
				TextBlock_MainMission->SetColorAndOpacity(FSlateColor(RedColor));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2"));
		}
	}
	// Game Phase - Second Match Preparation
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchPreparation_SecondTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (TextBlock_MissionDescription)
			{
				TextBlock_MissionDescription->SetText(DefendPreparationMissionText);
			}
			
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_MissionDescription)
			{
				TextBlock_MissionDescription->SetText(AttackPreparationMissionText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2"));
		}
	}
	// Game Phase - Second Team Offense 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
	{
		if (TextBlock_MissionDescription)
		{
			TextBlock_MissionDescription->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// Game Phase - Post Match 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_PostMatch)
	{
		RemoveFromParent(); 
	}

	bMissionDescriptionUpdated = true; 
}

void UHeroSelectionOverlay::NativeDestruct()
{
	// Remove Binding of Hero Name Changed Delegate 
	if (WBP_HeroSelectionList)
	{
		WBP_HeroSelectionList->HeroSelectButtonDelegate.RemoveAll(this);
	}

	// Remove Binding of Team Changed Delegate 
	if (OwnerPlayerState)
	{
		OwnerPlayerState->GetTeamChangedDelegate().RemoveAll(this);
	}

	// Unregister Gameplay Message Listener Handles - First Hero Selection Game Phase
	if (FirstHeroSelectionListenerHandle.IsValid())
	{
		FirstHeroSelectionListenerHandle.Unregister();
	}

	// Unregister Gameplay Message Listener Handles - First Match Preparation Game Phase
	if (FirstMatchPreparationListenerHandle.IsValid())
	{
		FirstMatchPreparationListenerHandle.Unregister();
	}

	// Unregister Gameplay Message Listener Handles - First Team Offense Game Phase
	if (FirstTeamOffenseListenerHandle.IsValid())
	{
		FirstTeamOffenseListenerHandle.Unregister();
	}

	// Unregister Gameplay Message Listener Handles - Second Hero Selection Game Phase
	if (SecondHeroSelectionListenerHandle.IsValid())
	{
		SecondHeroSelectionListenerHandle.Unregister();
	}

	// Unregister Gameplay Message Listener Handles - Second Match Preparation Game Phase
	if (SecondMatchPreparationListenerHandle.IsValid())
	{
		SecondMatchPreparationListenerHandle.Unregister();
	}

	// Unregister Gameplay Message Listener Handles - Second Team Offense Game Phase
	if (SecondTeamOffenseListenerHandle.IsValid())
	{
		SecondTeamOffenseListenerHandle.Unregister();
	}

	// Unregister Gameplay Message Listener Handles - Post Match Game Phase
	if (PostMatchListenerHandle.IsValid())
	{
		PostMatchListenerHandle.Unregister();
	}

	Super::NativeDestruct(); 
}

void UHeroSelectionOverlay::OnHeroSelectButtonClicked(EHeroName HeroName, bool bConfirmed)
{
	// Check if Hero Name is None 
	if (HeroName == EHeroName::None)
	{
		if (TextBlock_HeroName)
		{
			TextBlock_HeroName->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (TextBlock_WaitingForBattle && (TextBlock_WaitingForBattle->GetVisibility() == ESlateVisibility::Visible))
		{
			TextBlock_WaitingForBattle->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (WBP_HeroSelectionList && (WBP_HeroSelectionList->GetVisibility() != ESlateVisibility::Visible))
		{
			WBP_HeroSelectionList->SetVisibility(ESlateVisibility::Visible); 
		}
	}
	else
	{
		UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this);
		if (HeroInfo->HeroInformation.Find(HeroName))
		{
			SelectedHeroName = HeroName; 
			if (TextBlock_HeroName)
			{
				TextBlock_HeroName->SetText(HeroInfo->HeroInformation[HeroName].HeroDisplayName);
			}
			if (TextBlock_WaitingForBattle)
			{
				TextBlock_WaitingForBattle->SetVisibility(ESlateVisibility::Visible);
			}
			if (WBP_HeroSelectionList && (WBP_HeroSelectionList->GetVisibility() == ESlateVisibility::Visible))
			{
				WBP_HeroSelectionList->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

FText UHeroSelectionOverlay::ConvertTimeText(float RemainingCountdownTime)
{
	int32 TotalSeconds = FMath::FloorToInt(RemainingCountdownTime);
	FString TimeString = FString::Printf(TEXT("%d"), TotalSeconds);
	return FText::FromString(TimeString);
}

