// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameState.h"
#include "Team/TeamCreationComponent.h"
#include "Game/PlayerSpawningManagerComponent.h"
#include "Game/MatchScoringComponent.h"
#include "Game/PlayerPerformanceComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "Message/OWMessageTypes.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "OWGameplayTags.h"


AOWGameState::AOWGameState()
{
	PrimaryActorTick.bCanEverTick = true; 
	PrimaryActorTick.bStartWithTickEnabled = true; 

	OWAbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("OWAbilitySystemComponent");
	OWAbilitySystemComponent->SetIsReplicated(true);
	OWAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PlayerPerformanceComponent = CreateDefaultSubobject<UPlayerPerformanceComponent>("PlayerPerformanceComponent"); 
	PlayerPerformanceComponent->SetIsReplicated(true); 
}

void AOWGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
}

void AOWGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents(); 

	// Server-only Component 
	if (HasAuthority())
	{
		if (TeamCreationComponentClass)
		{
			// Create New Object, Replicate, and Register
			TeamCreationComponent = NewObject<UTeamCreationComponent>(this, TeamCreationComponentClass);
			TeamCreationComponent->SetIsReplicated(true);
			TeamCreationComponent->RegisterComponent();
		}

		// Create New Object, Replicate, and Register
		PlayerSpawningManagerComponent = NewObject<UPlayerSpawningManagerComponent>(this);
		PlayerSpawningManagerComponent->SetIsReplicated(true);
		PlayerSpawningManagerComponent->RegisterComponent();

		if (MatchScoringComponentClass)
		{
			// Create New Object, Replicate, and Register
			MatchScoringComponent = NewObject<UMatchScoringComponent>(this, MatchScoringComponentClass);
			MatchScoringComponent->SetIsReplicated(true);
			MatchScoringComponent->RegisterComponent();
		}
	}

	if (OWAbilitySystemComponent)
	{
		OWAbilitySystemComponent->InitAbilityActorInfo(this, this); 
	}
}

void AOWGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	OnPlayerStateAdded.Broadcast(PlayerState); 
}

void AOWGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	OnPlayerStateRemoved.Broadcast(PlayerState); 
}

void AOWGameState::MulticastReliableMessageToClients_Implementation(const FOWVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}
	