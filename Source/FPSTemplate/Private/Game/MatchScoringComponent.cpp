// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MatchScoringComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "Message/OWMessageTypes.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "Character/OWCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "OWGameplayTags.h"
#include "Team/OWTeamSubsystem.h"


UMatchScoringComponent::UMatchScoringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMatchScoringComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMatchScoringComponent, AssaultPoints);
}

void UMatchScoringComponent::RegisterAssaultPoint(AAssaultPoint* AssaultPoint)
{
	AssaultPoints.Add(AssaultPoint);
	OnAssaultPointRegistered.Broadcast(AssaultPoint); 
}

void UMatchScoringComponent::ContestPoint(AAssaultPoint* AssaultPoint)
{
	if (GetOwner()->HasAuthority())
	{
		AssaultPoints.Find(AssaultPoint); 
	}
}

int32 UMatchScoringComponent::GetTeamScore(int32 TeamId) const
{
	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
	if (TeamSubsystem)
	{
		int32 TeamScore = TeamSubsystem->GetTeamTagStackCount(TeamId, FOWGameplayTags::Get().BattleField_Assault_TeamScore); 
		return TeamScore; 
	}

	return 0; 
}

void UMatchScoringComponent::OnHeroSelectionStarted(UOWGamePhaseAbility* GamePhase)
{
	ResetAllActivePlayers(); 
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

void UMatchScoringComponent::ScorePoints()
{
	// gameplay message subsystem - 
}

void UMatchScoringComponent::HandleVictory(int32 TeamID)
{
	// Set Winning Team 
	WinningTeamID = TeamID; 
	
	// Replace Ability Level of the GameplayCueParameters with Team ID. 
	FGameplayCueParameters GameplayCueParameters; 
	GameplayCueParameters.AbilityLevel = WinningTeamID;
	ActivateMatchDecidedGameplayCue(FOWGameplayTags::Get().GameplayCue_MatchDecided, GameplayCueParameters);

	// Get Game Phase Subsystem
	UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
	if (GamePhaseSubsystem)
	{
		// Start Post Match Phase 
		FOWGamePhaseDelegate PostMatchEndedDelegate; 
		GamePhaseSubsystem->StartPhase(PostMatch, PostMatchEndedDelegate); 
	}
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

void UMatchScoringComponent::OnRep_AssaultPoints()
{
	for (AAssaultPoint* AssaultPoint : AssaultPoints)
	{
		OnAssaultPointRegistered.Broadcast(AssaultPoint);
	}
}



