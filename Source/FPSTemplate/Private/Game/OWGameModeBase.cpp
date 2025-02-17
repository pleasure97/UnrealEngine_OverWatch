// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameModeBase.h"
#include "Game/OWGameState.h"

void AOWGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 

	if (AOWGameState* OWGameState = GetGameState<AOWGameState>())
	{
		OWGameState->SetSelectedHeroName(SelectedHeroName); 
	}
}
