// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultOverlay.h"
#include "Game/MatchScoringComponent.h"
#include "Actor/AssaultPoint.h"
#include "OWGameplayTags.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "Team/OWTeamSubsystem.h"
#include "Team/AsyncAction_ObserveTeam.h"
#include "UI/Widget/Assault/AssaultProgress.h"
#include "UI/Widget/Assault/AssaultTimer.h"
#include "UI/Widget/Assault/AssaultScore.h"
#include "Components/TextBlock.h"

void UAssaultOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Assume that Team ID will not be changed until the game ends 
	CurrentOffenseTeamID = 1; 

	check(WidgetController);

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 
	MatchScoringComponentListener = GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
		FOWGameplayTags::Get().Gameplay_Message_MatchScoringComponent, this, &UAssaultOverlay::OnMatchScoringComponentRegistered);

	// Get Game Phase Subsystem
	UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>(); 
	if (GamePhaseSubsystem)
	{
		// Bind the Callback that is Activated When Switch Inning Phase Ends 
		GamePhaseSubsystem->WhenPhaseEnds(
			FOWGameplayTags::Get().GamePhase_SwitchInning,
			EPhaseTagMatchType::ExactMatch,
			FOWGamePhaseTagDelegate::CreateUObject(this, &UAssaultOverlay::OnSwitchInningEnded)); 

		// Bind the Callback that is Activated When First Team Attacks 
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_MatchInProgress_FirstTeamOffense,
			EPhaseTagMatchType::ExactMatch,
			FOWGamePhaseTagDelegate::CreateUObject(this, &UAssaultOverlay::OnFirstTeamOffenseStarted));

		// Bind the Callback that is Activated When Second Team Attacks 
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_MatchInProgress_SecondTeamOffense,
			EPhaseTagMatchType::ExactMatch,
			FOWGamePhaseTagDelegate::CreateUObject(this, &UAssaultOverlay::OnSecondTeamOffenseStarted));
	}

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

		if (MatchScoringComponentListener.IsValid())
		{
			MatchScoringComponentListener.Unregister();
		}
	}
	// Remove the Async Action Observe Team Binding 
	UAsyncAction_ObserveTeam* ObserveTeam = UAsyncAction_ObserveTeam::ObserveTeam(GetOwningPlayer());
	if (ObserveTeam)
	{
		ObserveTeam->OnTeamChanged.RemoveAll(this); 
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
		WBP_AssaultProgress->UpdateProgressDesign(CurrentOffenseTeamID);
	}

	OwnerTeamID = TeamID;
}


void UAssaultOverlay::UpdateOffensePreparation()
{
	if (TextBlock_Mission)
	{
		TextBlock_Mission->SetText(PrepareOffenseText);
	}
	if (Ally_AssaultTimer)
	{
		Ally_AssaultTimer->UpdateTeamColor(BlueTeamColor);
	}
	if (Ally_AssaultScore)
	{
		Ally_AssaultScore->UpdateOffenseDesign(BlueTeamColor);
	}
	if (Enemy_AssaultTimer)
	{
		Enemy_AssaultTimer->UpdateTeamColor(NeutralColor);
	}
	if (Enemy_AssaultScore)
	{
		Enemy_AssaultScore->UpdateDefenseDesign(RedTeamColor);
	}
}

void UAssaultOverlay::UpdateDefensePreparation()
{
	if (TextBlock_Mission)
	{
		TextBlock_Mission->SetText(PrepareDefenseText);
	}
	if (Ally_AssaultTimer)
	{
		Ally_AssaultTimer->UpdateTeamColor(NeutralColor);
	}
	if (Ally_AssaultScore)
	{
		Ally_AssaultScore->UpdateDefenseDesign(BlueTeamColor);
	}
	if (Enemy_AssaultTimer)
	{
		Enemy_AssaultTimer->UpdateTeamColor(RedTeamColor);
	}
	if (Enemy_AssaultScore)
	{
		Enemy_AssaultScore->UpdateOffenseDesign(RedTeamColor);
	}
}

void UAssaultOverlay::OnSwitchInningEnded(const FGameplayTag& PhaseTag)
{
	CurrentOffenseTeamID = 2; 

	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
	if (TeamSubsystem)
	{
		int32 TeamID = TeamSubsystem->FindTeamFromObject(GetOwningPlayer());
		// Team Color - Blue
		if (TeamID == 1)
		{
			UpdateDefensePreparation(); 
		}
		// Team Color - Red 
		else if (TeamID == 2)
		{
			UpdateOffensePreparation(); 
		}

		if (WBP_AssaultProgress)
		{
			WBP_AssaultProgress->UpdateProgressDesign(CurrentOffenseTeamID);
		}
	}
}

void UAssaultOverlay::OnFirstTeamOffenseStarted(const FGameplayTag& PhaseTag)
{
	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
	if (TeamSubsystem)
	{
		int32 TeamID = TeamSubsystem->FindTeamFromObject(GetOwningPlayer());
		// Team Color - Blue
		if (TeamID == 1)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(OffenseText); 
			}
		}
		// Team Color - Red 
		else if (TeamID == 2)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(DefenseText);
			}
		}
	}
}

void UAssaultOverlay::OnSecondTeamOffenseStarted(const FGameplayTag& PhaseTag)
{
	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
	if (TeamSubsystem)
	{
		int32 TeamID = TeamSubsystem->FindTeamFromObject(GetOwningPlayer());
		// Team Color - Blue
		if (TeamID == 1)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(DefenseText);
			}
		}
		// Team Color - Red 
		else if (TeamID == 2)
		{
			if (TextBlock_Mission)
			{
				TextBlock_Mission->SetText(OffenseText);
			}
		}
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
	if (CurrentOffenseTeamID <= 0 || OwnerTeamID <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Current Offense Team ID or Owner Team ID is not initialized in Assault Overlay.")); 
		return;
	}

	if (WBP_AssaultProgress)
	{
		WBP_AssaultProgress->UpdateOccupationProgress(NewOccupationProgress);
	}

	if (CurrentOffenseTeamID == OwnerTeamID)
	{
		if (Ally_AssaultTimer)
		{
			Ally_AssaultTimer->UpdateAssaultProgress(NewOccupationProgress);
		}
	}
	else
	{
		if (Enemy_AssaultTimer)
		{
			Enemy_AssaultTimer->UpdateAssaultProgress(NewOccupationProgress); 
		}
	}
}

void UAssaultOverlay::OnOccupationStateChanged(EOccupationState NewOccupationState)
{
	if (WBP_AssaultProgress)
	{
		WBP_AssaultProgress->UpdateOccupationState(NewOccupationState);
	}
}

void UAssaultOverlay::OnMatchScoringComponentRegistered(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	if (Payload.Instigator)
	{
		if (UMatchScoringComponent* RegisteredMatchScoringComponent = Payload.Instigator->GetComponentByClass<UMatchScoringComponent>())
		{
			MatchScoringComponent = RegisteredMatchScoringComponent; 
			MatchScoringComponent->OnAssaultPointRegistered.AddUObject(this, &UAssaultOverlay::OnAssaultPointRegistered);
		}
	}

}
