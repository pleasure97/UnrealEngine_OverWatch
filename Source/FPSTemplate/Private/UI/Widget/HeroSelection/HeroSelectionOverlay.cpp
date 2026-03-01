// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelectionOverlay.h"
#include "OWGameplayTags.h"
#include "Player/OWPlayerState.h"
#include "Player/OWPlayerController.h"
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
		OnHeroSelectButtonClicked(EHeroName::None, false); 
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
		UE_LOG(LogTemp, Log, TEXT("Hero Selection List in UHeroSelectionOverlay::NativeConstruct()"));
		WBP_HeroSelectionList->InitializeHeroSelectionList(); 
	}

	// Get Team Subsystem from the World 
	if (UWorld* World = GetWorld())
	{
		if (UOWTeamSubsystem* TeamSubsystem = World->GetSubsystem<UOWTeamSubsystem>())
		{
			// Bind Team Changed Delegate of Custom Player Controller
			if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetOwningPlayer()))
			{
				OWPlayerController->GetTeamChangedDelegate().AddDynamic(this, &UHeroSelectionOverlay::OnTeamChanged); 
			}
		}

		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

		// Register Gameplay Message Listener Handles
		RegisterGamePhaseListener(FirstHeroSelectionListenerHandle, GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, this, &UHeroSelectionOverlay::HandleCountdownTime);
		RegisterGamePhaseListener(FirstMatchPreparationListenerHandle, GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		RegisterGamePhaseListener(FirstTeamOffenseListenerHandle, GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		RegisterGamePhaseListener(SecondHeroSelectionListenerHandle, GameplayTags.GamePhase_HeroSelection_SecondHeroSelection, this, &UHeroSelectionOverlay::HandleCountdownTime);
		RegisterGamePhaseListener(SecondTeamOffenseListenerHandle, GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense, this, &UHeroSelectionOverlay::HandleCountdownTime);
		RegisterGamePhaseListener(PostMatchListenerHandle, GameplayTags.GamePhase_PostMatch, this, &UHeroSelectionOverlay::HandleCountdownTime);
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

	UpdateWidgetForCurrentPhase();
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
	
	UpdateWidgetForCurrentPhase();

	bMissionDescriptionUpdated = true; 
}

void UHeroSelectionOverlay::UpdateWidgetForCurrentPhase()
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

	if (OwnerTeamID != 1 && OwnerTeamID != 2)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2"));
		return;
	}

	// Game Phase - First Hero Selection 
	if (CurrentGamePhaseTag == GameplayTags.GamePhase_HeroSelection_FirstHeroSelection)
	{
		UpdateWidgetForHeroSelectionPhase(true);
	}
	// Game Phase - First Match Preparation 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense)
	{
		UpdateWidgetForMatchPreparationPhase(true);
	}
	// Game Phase - First Team Offense
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense)
	{
		UpdateWidgetForMatchPhase();
	}
	// Game Phase - Second Hero Selection
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_HeroSelection_SecondHeroSelection)
	{
		UpdateWidgetForHeroSelectionPhase(false);
	}
	// Game Phase - Second Match Preparation
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchPreparation_SecondTeamOffense)
	{
		UpdateWidgetForMatchPreparationPhase(false);
	}
	// Game Phase - Second Team Offense 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
	{
		UpdateWidgetForMatchPhase();
	}
	// Game Phase - Post Match 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_PostMatch)
	{
		RemoveFromParent();
	}
}

void UHeroSelectionOverlay::UpdateWidgetForHeroSelectionPhase(bool bFirstTeamAttack)
{
	if (TextBlock_MissionDescription)
	{
		TextBlock_MissionDescription->SetText(WaitingForTeamConstructionText);
		TextBlock_MissionDescription->SetVisibility(ESlateVisibility::Visible);
	}

	if (TextBlock_RemainingTime)
	{
		TextBlock_RemainingTime->SetVisibility(ESlateVisibility::Visible);
	}

	bool bIsCurrentlyAttacking = (bFirstTeamAttack && OwnerTeamID == 1) || (!bFirstTeamAttack && OwnerTeamID == 2);

	FText MainMissionText = bIsCurrentlyAttacking ? AttackMissionText : DefendMissionText;
	FSlateColor MainMissionTextColor = bIsCurrentlyAttacking ? FSlateColor(RedColor) : FSlateColor(BlueColor);

	if (TextBlock_MainMission)
	{
		TextBlock_MainMission->SetText(MainMissionText);
		TextBlock_MainMission->SetColorAndOpacity(MainMissionTextColor);
	}
}

void UHeroSelectionOverlay::UpdateWidgetForMatchPreparationPhase(bool bFirstTeamAttack)
{
	bool bIsCurrentlyAttacking = (bFirstTeamAttack && OwnerTeamID == 1) || (!bFirstTeamAttack && OwnerTeamID == 2);

	FText MissionPreparationText = bIsCurrentlyAttacking ? AttackPreparationMissionText : DefendPreparationMissionText;
	if (TextBlock_MissionDescription)
	{
		TextBlock_MissionDescription->SetText(MissionPreparationText);
	}
}

void UHeroSelectionOverlay::UpdateWidgetForMatchPhase()
{
	if (TextBlock_MissionDescription)
	{
		TextBlock_MissionDescription->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (TextBlock_RemainingTime)
	{
		TextBlock_RemainingTime->SetVisibility(ESlateVisibility::Collapsed);
	}
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

	// Unregister Gameplay Message Listener Handles
	UnregisterGamePhaseListener(FirstHeroSelectionListenerHandle);
	UnregisterGamePhaseListener(FirstMatchPreparationListenerHandle);
	UnregisterGamePhaseListener(SecondHeroSelectionListenerHandle);
	UnregisterGamePhaseListener(SecondMatchPreparationListenerHandle);
	UnregisterGamePhaseListener(SecondTeamOffenseListenerHandle);
	UnregisterGamePhaseListener(PostMatchListenerHandle);

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
		if (TextBlock_HeroName->GetVisibility() == ESlateVisibility::Visible)
		{
			TextBlock_HeroName->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(GetWorld());
		if (!IsValid(HeroInfo))
		{
			UE_LOG(LogTemp, Error, TEXT("Hero Info is Not Valid in UHeroSelectionOverlay::OnHeroSelectButtonClicked()"));
			return;
		}
		
		if (HeroInfo->HeroInformation.Find(HeroName))
		{
			SelectedHeroName = HeroName; 
			if (TextBlock_HeroName)
			{
				TextBlock_HeroName->SetText(HeroInfo->HeroInformation[HeroName].HeroDisplayName);
				if (TextBlock_HeroName->GetVisibility() != ESlateVisibility::Visible)
				{
					TextBlock_HeroName->SetVisibility(ESlateVisibility::Visible);
				}
			}
			if (TextBlock_WaitingForBattle)
			{
				TextBlock_WaitingForBattle->SetVisibility(ESlateVisibility::Visible);
			}
		}

		UpdateVisibilityBasedOnGamePhase();
	}
}

void UHeroSelectionOverlay::UpdateVisibilityBasedOnGamePhase()
{
	// Get Game Phase Subsystem
	UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
	if (!IsValid(GamePhaseSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("GamePhaseSubsystem is Not Valid in HeroSelectionOverlay::OnHeroSelectButtonClicked()"));
		return;
	}

	// Check if GamePhase is 'Match In Progress' or 'Match Preparation'
	FGameplayTag MatchInProgressTag = FGameplayTag::RequestGameplayTag(FName("GamePhase.MatchInProgress"));
	FGameplayTag MatchPreparationTag = FGameplayTag::RequestGameplayTag(FName("GamePhase.MatchPreparation"));
	if (GamePhaseSubsystem->IsPhaseActive(MatchInProgressTag) || GamePhaseSubsystem->IsPhaseActive(MatchPreparationTag))
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

FText UHeroSelectionOverlay::ConvertTimeText(float RemainingCountdownTime)
{
	int32 TotalSeconds = FMath::FloorToInt(RemainingCountdownTime);
	FString TimeString = FString::Printf(TEXT("%d"), TotalSeconds);
	return FText::FromString(TimeString);
}

