// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultOverlay.h"
#include "Game/MatchScoringComponent.h"
#include "Actor/AssaultPoint.h"
#include "OWGameplayTags.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "Team/OWTeamSubsystem.h"
#include "Team/AsyncAction_ObserveTeam.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/Assault/AssaultProgress.h"
#include "UI/Widget/Assault/AssaultScoreDetails.h"
#include "UI/Widget/Assault/AssaultScore.h"
#include "Components/TextBlock.h"

void UAssaultOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Get Observe Team Async Action 
	UAsyncAction_ObserveTeam* ObserveTeam = UAsyncAction_ObserveTeam::ObserveTeam(GetOwningPlayer()); 
	if (ObserveTeam)
	{
		// Bind the callback that is Activated When Team is Changed 
		ObserveTeam->OnTeamChanged.AddDynamic(this, &UAssaultOverlay::OnTeamChanged);

		if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
		{
			int32 TeamID = TeamSubsystem->FindTeamFromObject(GetOwningPlayer()); 
			if (TeamID > 0)
			{
				OnTeamChanged(true, TeamID); 
			}
		}
	}

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// Get GameplayTag Singleton Class 
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	// Register Game Phase Listeners to Gameplay Message Subsystem 
	FirstHeroSelectionListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, this, &UAssaultOverlay::HandleCountdownTime);
	FirstMatchPreparationListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense, this, &UAssaultOverlay::HandleCountdownTime);
	FirstTeamOffenseListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense, this, &UAssaultOverlay::HandleCountdownTime);
	SwitchInningListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_SwitchInning, this, &UAssaultOverlay::HandleCountdownTime);
	SecondHeroSelectionListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_HeroSelection_SecondHeroSelection, this, &UAssaultOverlay::HandleCountdownTime);
	SecondMatchPreparationListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_MatchPreparation_SecondTeamOffense, this, &UAssaultOverlay::HandleCountdownTime);
	SecondTeamOffenseListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense, this, &UAssaultOverlay::HandleCountdownTime);
	PostMatchListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			GameplayTags.GamePhase_PostMatch, this, &UAssaultOverlay::HandleCountdownTime);
}

void UAssaultOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 

	// Allow Assault Point to be Registered Only Once by Getting Match Scoring Component of Game State 
	if (!bAssaultPointRegistered)
	{
		if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
		{
			if (UMatchScoringComponent* FoundMatchScoringComponent = GameState->FindComponentByClass<UMatchScoringComponent>())
			{
				// Save Match Scoring Component as Member Variable 
				MatchScoringComponent = FoundMatchScoringComponent;
				// Bind Assault Point Registeration Delegate of Match Scoring Component 
				MatchScoringComponent->OnAssaultPointRegistered.AddUObject(this, &UAssaultOverlay::OnAssaultPointRegistered);
				// Call Callback Function to Function Properly 
				for (AAssaultPoint* RegisterdAssaultPoint : MatchScoringComponent->AssaultPoints)
				{
					OnAssaultPointRegistered(RegisterdAssaultPoint);
				}
			}
			bAssaultPointRegistered = true; 
		}
		else
		{
			return;
		}
	}

	if (FMath::IsNearlyEqual(DisplayedProgress, TargetProgress))
	{
		return;
	}

	// Interpolate from Displayed Progress to Target Progress So that it goes up gradually rather than all at once
	DisplayedProgress = FMath::FInterpTo(DisplayedProgress, TargetProgress, InDeltaTime, 1.f); 

	// Get Current Game Phase Tag and Update Assault Progress to Match Current Game Phase 
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	// Game Phase - First Team Offense 
	if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (Ally_AssaultScoreDetails)
			{
				Ally_AssaultScoreDetails->UpdateAssaultProgress(DisplayedProgress);
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (Enemy_AssaultScoreDetails)
			{
				Enemy_AssaultScoreDetails->UpdateAssaultProgress(DisplayedProgress);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2 in First Team Offense in UAssaultOverlay::ProcessPhaseRemainingTime()"));
		}
	}
	// Game Phase - Second Team Offense 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (Enemy_AssaultScoreDetails)
			{
				Enemy_AssaultScoreDetails->UpdateAssaultProgress(DisplayedProgress);
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (Ally_AssaultScoreDetails)
			{
				Ally_AssaultScoreDetails->UpdateAssaultProgress(DisplayedProgress);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2 in Second Team Offense in UAssaultOverlay::ProcessPhaseRemainingTime()"));
		}
	}
}

void UAssaultOverlay::NativeDestruct()
{
	if (MatchScoringComponent)
	{
		// Iterate Match Scoring Component's Assault Points Array and Remove Related Bindings 
		for (AAssaultPoint* AssaultPoint : MatchScoringComponent->AssaultPoints)
		{
			AssaultPoint->OnNumAttackersChanged.RemoveAll(this); 
			AssaultPoint->OnNumDefendersChanged.RemoveAll(this);
			AssaultPoint->OnOccupationProgressChanged.RemoveAll(this);
			AssaultPoint->OnOccupationStateChanged.RemoveAll(this);
		}
	}
	// Remove the Async Action Observe Team Binding 
	UAsyncAction_ObserveTeam* ObserveTeam = UAsyncAction_ObserveTeam::ObserveTeam(GetOwningPlayer());
	if (ObserveTeam)
	{
		ObserveTeam->OnTeamChanged.RemoveAll(this); 
	}

	// Unregister All Game Phase Listener Handles 
	if (FirstHeroSelectionListenerHandle.IsValid())
	{
		FirstHeroSelectionListenerHandle.Unregister();
	}

	if (FirstMatchPreparationListenerHandle.IsValid())
	{
		FirstMatchPreparationListenerHandle.Unregister();
	}

	if (FirstTeamOffenseListenerHandle.IsValid())
	{
		FirstTeamOffenseListenerHandle.Unregister();
	}

	if (SwitchInningListenerHandle.IsValid())
	{
		SwitchInningListenerHandle.Unregister();
	}

	if (SecondHeroSelectionListenerHandle.IsValid())
	{
		SecondHeroSelectionListenerHandle.Unregister();
	}

	if (SecondMatchPreparationListenerHandle.IsValid())
	{
		SecondMatchPreparationListenerHandle.Unregister();
	}

	if (SecondTeamOffenseListenerHandle.IsValid())
	{
		SecondTeamOffenseListenerHandle.Unregister();
	}

	if (PostMatchListenerHandle.IsValid())
	{
		PostMatchListenerHandle.Unregister();
	}

	Super::NativeDestruct(); 
}

void UAssaultOverlay::OnAssaultPointRegistered(AAssaultPoint* AssaultPoint)
{
	if (!IsValid(AssaultPoint))
	{
		return; 
	}

	AssaultPoint->OnNumAttackersChanged.AddDynamic(this, &UAssaultOverlay::OnNumAttackersChanged);
	AssaultPoint->OnNumDefendersChanged.AddDynamic(this, &UAssaultOverlay::OnNumDefendersChanged);
	AssaultPoint->OnOccupationProgressChanged.AddDynamic(this, &UAssaultOverlay::OnOccupationProgressChanged);
	AssaultPoint->OnOccupationStateChanged.AddDynamic(this, &UAssaultOverlay::OnOccupationStateChanged);

	// Force to Call Initial Values of Assault Point 
	OnNumAttackersChanged(AssaultPoint->GetNumAttackers()); 
	OnNumDefendersChanged(AssaultPoint->GetNumDefenders());
	OnOccupationProgressChanged(AssaultPoint->GetOccupationProgress());
	OnOccupationStateChanged(AssaultPoint->GetOccupationState());
}

void UAssaultOverlay::OnTeamChanged(bool bTeamSet, int32 TeamID)
{
	// Team Color - Blue
	if (TeamID == 1)
	{
		UpdateOffensePreparation();
	}
	// Team Color - Red 
	else if (TeamID == 2)
	{
		UpdateDefensePreparation();
	}

	if (WBP_AssaultProgress)
	{
		// TODO - Change UpdateProgressDesign()
		// WBP_AssaultProgress->UpdateProgressDesign(CurrentOffenseTeamID);
	}

	OwnerTeamID = TeamID;
}

void UAssaultOverlay::ProcessPhaseRemainingTime(const FGameplayTag& PhaseTag, const float RemainingTime)
{
	// Set New Current Game Phase Tag and Prepare to Update Mission Description
	if (!CurrentGamePhaseTag.MatchesTagExact(PhaseTag))
	{
		CurrentGamePhaseTag = PhaseTag; 
		bMissionUpdated = false;
	}

	// Update Remaining Time in Any Game Phase 
	FText RemainingTimeText = ConvertTimeText(RemainingTime);
	if (TextBlock_RoundRemainingTime)
	{
		TextBlock_RoundRemainingTime->SetText(RemainingTimeText);
	}

	// Get GameplayTag Singleton Class
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (Ally_AssaultScoreDetails)
			{
				Ally_AssaultScoreDetails->UpdateRemainingTime(RemainingTimeText); 
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (Enemy_AssaultScoreDetails)
			{
				Enemy_AssaultScoreDetails->UpdateRemainingTime(RemainingTimeText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2 in First Team Offense in UAssaultOverlay::ProcessPhaseRemainingTime()")); 
		}
	}
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (Enemy_AssaultScoreDetails)
			{
				Enemy_AssaultScoreDetails->UpdateRemainingTime(RemainingTimeText);
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (Ally_AssaultScoreDetails)
			{
				Ally_AssaultScoreDetails->UpdateRemainingTime(RemainingTimeText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2 in Second Team Offense in UAssaultOverlay::ProcessPhaseRemainingTime()"));
		}
	}

	// Early return if Mission Description is Updated 
	if (bMissionUpdated)
	{
		return;
	}

	// Game Phase - First Hero Selection 
	if (CurrentGamePhaseTag == GameplayTags.GamePhase_HeroSelection_FirstHeroSelection)
	{
		if (OwnerTeamID == 1)
		{ 
			UpdateOffensePreparation(); 
		}
		else if (OwnerTeamID == 2)
		{
			UpdateDefensePreparation(); 
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2."));
		}
	}
	// Game Phase - First Match Preparation 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchPreparation_FirstTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(PrepareOffenseText);
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(PrepareDefenseText);
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
		if (OwnerTeamID == 1)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(OffenseText);
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(DefenseText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2"));
		}
	}
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_SwitchInning)
	{
		DisplayedProgress = 0.f; 
		TargetProgress = 0.f; 
	}
	// Game Phase - Second Hero Selection
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_HeroSelection_SecondHeroSelection)
	{
		// Set Design 
		if (OwnerTeamID == 1)
		{
			UpdateDefensePreparation(); 
		}
		else if (OwnerTeamID == 2)
		{
			UpdateOffensePreparation(); 
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2."));
		}
	}
	// Game Phase - Second Match Preparation
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchPreparation_SecondTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(PrepareDefenseText);
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(PrepareOffenseText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2."));
		}
	}
	// Game Phase - Second Team Offense 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
	{
		if (OwnerTeamID == 1)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(DefenseText); 
			}
		}
		else if (OwnerTeamID == 2)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(OffenseText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2.")); 
		}
	}
	// Game Phase - Post Match 
	else if (CurrentGamePhaseTag == GameplayTags.GamePhase_PostMatch)
	{
		if (TextBlock_Mission)
		{
			TextBlock_Mission->SetText(RoundEndText); 
		}
	}

	bMissionUpdated = true;
}

void UAssaultOverlay::UpdateOffensePreparation()
{
	// Update Offense Mission Text and UI Color 
	if (TextBlock_Mission)
	{
		TextBlock_Mission->SetText(PrepareOffenseText);
	}
	if (Ally_AssaultScoreDetails)
	{
		Ally_AssaultScoreDetails->UpdateTeamColor(BlueTeamColor);
	}
	if (Ally_AssaultScore)
	{
		Ally_AssaultScore->UpdateOffenseDesign(BlueTeamColor);
	}
	if (Enemy_AssaultScoreDetails)
	{
		Enemy_AssaultScoreDetails->UpdateTeamColor(NeutralColor);
	}
	if (Enemy_AssaultScore)
	{
		Enemy_AssaultScore->UpdateDefenseDesign(RedTeamColor);
	}
}

void UAssaultOverlay::UpdateDefensePreparation()
{
	// Update Defense Mission Text and UI Color 
	if (TextBlock_Mission)
	{
		TextBlock_Mission->SetText(PrepareDefenseText);
	}
	if (Ally_AssaultScoreDetails)
	{
		Ally_AssaultScoreDetails->UpdateTeamColor(NeutralColor);
	}
	if (Ally_AssaultScore)
	{
		Ally_AssaultScore->UpdateDefenseDesign(BlueTeamColor);
	}
	if (Enemy_AssaultScoreDetails)
	{
		Enemy_AssaultScoreDetails->UpdateTeamColor(RedTeamColor);
	}
	if (Enemy_AssaultScore)
	{
		Enemy_AssaultScore->UpdateOffenseDesign(RedTeamColor);
	}
}

void UAssaultOverlay::OnNumAttackersChanged(int32 NewNumAttackers)
{
	if (WBP_AssaultProgress)
	{
		WBP_AssaultProgress->UpdateNumAttackers(NewNumAttackers); 
	}
}

void UAssaultOverlay::OnNumDefendersChanged(int32 NewNumDefenders)
{
	if (WBP_AssaultProgress)
	{
		WBP_AssaultProgress->UpdateNumDefenders(NewNumDefenders);
	}
}

void UAssaultOverlay::OnOccupationProgressChanged(float NewOccupationProgress)
{
	if (OwnerTeamID <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not initialized in Assault Overlay.")); 
		return;
	}

	if (WBP_AssaultProgress)
	{
		WBP_AssaultProgress->UpdateOccupationProgress(NewOccupationProgress);
	}

	TargetProgress = NewOccupationProgress; 
}

void UAssaultOverlay::OnOccupationStateChanged(EOccupationState NewOccupationState)
{
	if (WBP_AssaultProgress)
	{
		WBP_AssaultProgress->UpdateOccupationState(NewOccupationState);
	}
}

void UAssaultOverlay::HandleCountdownTime(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	ProcessPhaseRemainingTime(Payload.Verb, Payload.Magnitude); 
}

FText UAssaultOverlay::ConvertTimeText(float RemainingCountdownTime)
{
	// e.g., 240 Seconds -> 4:00 / 14 Seconds -> 0:14 
	int32 TotalSeconds = FMath::FloorToInt(RemainingCountdownTime); 
	int32 Minutes = TotalSeconds / 60; 
	int32 Seconds = TotalSeconds % 60; 

	FString SecondsString = (Seconds < 10) ? FString::Printf(TEXT("0%d"), Seconds) : FString::FromInt(Seconds); 
	FString TimeString = FString::Printf(TEXT("%d:%s"), Minutes, *SecondsString); 

	return FText::FromString(TimeString); 
}
