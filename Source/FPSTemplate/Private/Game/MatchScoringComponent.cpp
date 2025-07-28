// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MatchScoringComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "Character/OWCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "OWGameplayTags.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/OWGameState.h"


UMatchScoringComponent::UMatchScoringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); 
}

void UMatchScoringComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMatchScoringComponent, AssaultPoints);
	DOREPLIFETIME(UMatchScoringComponent, CountdownTime);
}

void UMatchScoringComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		DelayTimerHandle,
		this,
		&UMatchScoringComponent::ConnectToGamePhase,
		3.f,
		false);
}

void UMatchScoringComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayTimerHandle); 
		World->GetTimerManager().ClearTimer(CountdownTimerHandle); 
	}

	if (FirstTeamOffensePhaseDelegate.IsBound())
	{
		FirstTeamOffensePhaseDelegate.Unbind();
	}

	if (FirstMatchPreparationDelegate.IsBound())
	{
		FirstMatchPreparationDelegate.Unbind();
	}

	if (FirstTeamOffensePhaseDelegate.IsBound())
	{
		FirstTeamOffensePhaseDelegate.Unbind();
	}

	if (SecondHeroSelectionGamePhaseDelegate.IsBound())
	{
		SecondHeroSelectionGamePhaseDelegate.Unbind();
	}

	if (SecondMatchPreparationDelegate.IsBound())
	{
		SecondMatchPreparationDelegate.Unbind();
	}

	if (SecondTeamOffensePhaseDelegate.IsBound())
	{
		SecondTeamOffensePhaseDelegate.Unbind();
	}

	Super::EndPlay(EndPlayReason); 
}

void UMatchScoringComponent::RegisterAssaultPoint(AAssaultPoint* AssaultPoint)
{
	AssaultPoints.Add(AssaultPoint);
	OnAssaultPointRegistered.Broadcast(AssaultPoint); 
}

void UMatchScoringComponent::OnRep_AssaultPoints()
{
	for (AAssaultPoint* AssaultPoint : AssaultPoints)
	{
		OnAssaultPointRegistered.Broadcast(AssaultPoint);
	}
}

void UMatchScoringComponent::OnRep_CountdownTime()
{
	OnPhaseRemainingTime.Broadcast(CurrentGamePhaseTag, CountdownTime); 
}

void UMatchScoringComponent::ConnectToGamePhase()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayTimerHandle);
	}

	if (GetOwner()->HasAuthority())
	{
		if (UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>())
		{
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

			// Start First Hero Selection Game Phase
			GamePhaseSubsystem->StartPhase(FirstHeroSelectionGamePhaseAbility); 

			// Connect to When First Hero Selection Game Phase Starts
			FirstHeroSelectionGamePhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleFirstHeroSelectionPhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_HeroSelection_FirstHeroSelection, EPhaseTagMatchType::ExactMatch, FirstHeroSelectionGamePhaseDelegate); 

			// Connect to When First Match Preparation Game Phase Starts
			FirstMatchPreparationDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleFirstMatchPreparationPhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_MatchPreparation_FirstTeamOffense, EPhaseTagMatchType::ExactMatch, FirstMatchPreparationDelegate);

			// Connect to When First Team Offesnse Game Phase Starts
			FirstTeamOffensePhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleFirstTeamOffensePhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_MatchInProgress_FirstTeamOffense, EPhaseTagMatchType::ExactMatch, FirstTeamOffensePhaseDelegate);

			// Connect to When Second Hero Selection Game Phase Starts
			SecondHeroSelectionGamePhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSecondHeroSelectionPhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_HeroSelection_SecondHeroSelection, EPhaseTagMatchType::ExactMatch, SecondHeroSelectionGamePhaseDelegate);

			// Connect to When Second Match Preparation Game Phase Starts
			SecondMatchPreparationDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSecondMatchPreparationPhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_MatchPreparation_SecondTeamOffense, EPhaseTagMatchType::ExactMatch, SecondMatchPreparationDelegate);

			// Connect to When Second Team Offesnse Game Phase Starts
			SecondTeamOffensePhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSecondTeamOffensePhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_MatchInProgress_SecondTeamOffense, EPhaseTagMatchType::ExactMatch, SecondTeamOffensePhaseDelegate);
		}
	}
}

void UMatchScoringComponent::Countdown()
{
	--CountdownTime; 

	if (AOWGameState* OWGameState = Cast<AOWGameState>(GetOwner()))
	{
		FOWVerbMessage OWVerbMessage; 
		OWVerbMessage.Verb = CurrentGamePhaseTag; 
		OWVerbMessage.Magnitude = CountdownTime;
		OWGameState->MulticastReliableMessageToClients(OWVerbMessage); 
	}

	if (CountdownTime <= 0.f)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(CountdownTimerHandle); 
		}
	}
}

void UMatchScoringComponent::GamePhaseStarted(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	CurrentGamePhaseTag = PhaseTag;
	CountdownTime = PhaseDuration;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UMatchScoringComponent::Countdown, 1.f, true);
	}
}

void UMatchScoringComponent::HandleFirstHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleFirstMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleFirstTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleSecondHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleSecondMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleSecondTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}


int32 UMatchScoringComponent::GetTeamScore(int32 TeamId) const
{
	if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		int32 TeamScore = TeamSubsystem->GetTeamTagStackCount(TeamId, FOWGameplayTags::Get().BattleField_Assault_TeamScore); 
		return TeamScore; 
	}

	return 0; 
}

void UMatchScoringComponent::ResetAllActivePlayers()
{
	// Declare Heroes Array 
	TArray<AActor*> AllHeroes;
	// Get All Heros that exist in the World
	UGameplayStatics::GetAllActorsOfClass(this, AOWCharacter::StaticClass(), AllHeroes);

	for (AActor* Hero : AllHeroes)
	{
		// Get Ability System Component Using Ability System Blueprint Library 
		if (UAbilitySystemComponent* HeroASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Hero))
		{
			// Check if Hero Has Ability System Component
			if (!HeroASC)
			{
				return;
			}

			// Check if Hero Has Controller
			APawn* HeroPawn = Cast<APawn>(Hero); 
			if (!HeroPawn->GetController())
			{
				return;
			}

			// Send Gameplay Event to Reset All Heroes
			FGameplayEventData Payload; 
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Hero, FOWGameplayTags::Get().Event_ResetHero, Payload); 
		}
	}
}

void UMatchScoringComponent::ScoreAssaultPoint(FOccupationInfo OccupationInfo)
{
	if ((OccupationInfo.OccupationTeamID != 1) && (OccupationInfo.OccupationTeamID != 2))
	{
		return;
	}

	if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		if (OccupationInfo.OccupationProgress >= 1.f)
		{
			TeamSubsystem->AddTeamTagStack(OccupationInfo.OccupationTeamID, FOWGameplayTags::Get().BattleField_Assault_TeamScore, 1); 
		}
	}
}

void UMatchScoringComponent::HandleVictory(int32 TeamID)
{
	// Set Winning Team 
	WinningTeamID = TeamID; 
	
	// Replace Ability Level of the GameplayCueParameters with Team ID. 
	FGameplayCueParameters GameplayCueParameters; 
	GameplayCueParameters.AbilityLevel = WinningTeamID;
	ActivateMatchDecidedGameplayCue(FOWGameplayTags::Get().GameplayCue_MatchDecided, GameplayCueParameters);
}

void UMatchScoringComponent::ActivateMatchDecidedGameplayCue(FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters)
{
	ClearMatchDecidedGameplayCue(); 

	// Check if GameplayCueTag is Valid 
	if (GameplayCueTag.IsValid())
	{
		MatchDecidedTag = GameplayCueTag;
		// Add Match Decided Gameplay Cue to Gameplay State using Gameplay Cue Function Library 
		if (AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(this))
		{
			UGameplayCueFunctionLibrary::AddGameplayCueOnActor(GameStateBase, MatchDecidedTag, GameplayCueParameters);
		}
	}
}

void UMatchScoringComponent::ClearMatchDecidedGameplayCue()
{
	// Check if Match Decided Tag Member Variable is Valid 
	if (MatchDecidedTag.IsValid())
	{
		// Remove Match Decided Gameplay Cue to Gameplay State using Gameplay Cue Function Library 
		if (AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(this))
		{
			UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(GameStateBase, MatchDecidedTag, FGameplayCueParameters()); 
			// Reset Match Decided Tag that is the Member Variable 
			MatchDecidedTag = FGameplayTag(); 
		}
	}
}