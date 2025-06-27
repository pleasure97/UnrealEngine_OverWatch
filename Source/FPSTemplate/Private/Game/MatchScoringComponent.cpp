// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MatchScoringComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "Character/OWCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"
#include "Team/OWTeamSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"


UMatchScoringComponent::UMatchScoringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMatchScoringComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMatchScoringComponent, AssaultPoints);
}

void UMatchScoringComponent::BeginPlay()
{
	Super::BeginPlay(); 

	// Check if Game State Has Authority 
	if (GetOwner()->HasAuthority())
	{
		// Listen for Hero Killed Gameplay Message 

		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);

		FGameplayTag RespawnTag = FOWGameplayTags::Get().Gameplay_Message_Respawn;

		/*FGameplayMessageListenerHandle ListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FHeroRespawnInfo>(RespawnTag, this, &UMatchScoringComponent::);*/

		// Check if World exists
		if (!GetWorld())
		{
			return;
		}
		// Get Game Phase Subsystem
		UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>(); 
		if (GamePhaseSubsystem)
		{
			// Initialize Game Phase Delegate 
			FOWGamePhaseDynamicDelegate HeroSelectionDelegate; 
			// Bind Game Phase Delegate to Callback
			HeroSelectionDelegate.BindUFunction(this, FName("OnHeroSelectionStarted"));
			// Start Phase 
			GamePhaseSubsystem->StartPhase(HeroSelection, HeroSelectionDelegate);
		}
	}
}

void UMatchScoringComponent::RegisterAssaultPoint(AAssaultPoint* AssaultPoint)
{
	AssaultPoints.Add(AssaultPoint); 
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
}

void UMatchScoringComponent::GetPointStatus(int32 TeamID)
{

}

void UMatchScoringComponent::HandleVictory(int32 TeamID)
{
	// Set Winning Team 
	WinningTeamID = TeamID; 
	
	FGameplayCueParameters GameplayCueParameters; 
	GameplayCueParameters.AbilityLevel = WinningTeamID;
	ActivateMatchDecidedGameplayCue(FOWGameplayTags::Get().GameplayCue_MatchDecided, GameplayCueParameters);

	// Get Game Phase Subsystem
	UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
	if (GamePhaseSubsystem)
	{
		// Start Post Match Phase 
		FOWGamePhaseDynamicDelegate PostMatchEndedDelegate; 
		GamePhaseSubsystem->StartPhase(PostMatch, PostMatchEndedDelegate); 
	}
}

void UMatchScoringComponent::ActivateMatchDecidedGameplayCue(FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters)
{

}

void UMatchScoringComponent::ClearMatchDecidedGameplayCue()
{

}



