// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameState.h"
#include "Net/UnrealNetwork.h"

void AOWGameState::OnRep_SelectedHeroName()
{
	UE_LOG(LogTemp, Log, TEXT("Hero changed to: %d"), static_cast<int32>(SelectedHeroName));
}

void AOWGameState::SetSelectedHeroName(EHeroName NewHeroName)
{
	if (HasAuthority())
	{
		SelectedHeroName = NewHeroName; 
		OnRep_SelectedHeroName(); 
	}
}

void AOWGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(AOWGameState, SelectedHeroName);
}
