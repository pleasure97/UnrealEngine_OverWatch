// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameState.h"
#include "Team/TeamCreationComponent.h"
#include "Game/PlayerSpawningManagerComponent.h"

AOWGameState::AOWGameState()
{
	PrimaryActorTick.bCanEverTick = true; 
	PrimaryActorTick.bStartWithTickEnabled = true; 
}

void AOWGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
}

void AOWGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents(); 

	if (TeamCreationComponentClass)
	{
		TeamCreationComponent = NewObject<UTeamCreationComponent>(this, TeamCreationComponentClass); 
		TeamCreationComponent->RegisterComponent(); 
	}

	if (PlayerSpawningManagerComponentClass)
	{
		PlayerSpawningManagerComponent = NewObject<UPlayerSpawningManagerComponent>(this, PlayerSpawningManagerComponentClass); 
		PlayerSpawningManagerComponent->RegisterComponent(); 
	}
}
