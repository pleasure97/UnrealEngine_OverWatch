// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWLocalPlayer.h"

bool UOWLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld); 

	OnPlayerControllerChanged(PlayerController); 

	return bResult; 
}

void UOWLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController); 

	Super::InitOnlineSession(); 
}

void UOWLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam; 
	if (ITeamInterface* ControllerAsTeamProvider = Cast<ITeamInterface>(LastBoundPlayerController.Get()))
	{
		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId(); 
		ControllerAsTeamProvider->GetTeamChangedDelegate().RemoveAll(this);
	}

	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam; 
	if (ITeamInterface* ControllerAsTeamProvider = Cast<ITeamInterface>(NewController))
	{
		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId(); 
		ControllerAsTeamProvider->GetTeamChangedDelegate().AddDynamic(this, &UOWLocalPlayer::OnControllerChangedTeam); 
		LastBoundPlayerController = NewController; 
	}

	BroadcastTeamChanged(this, OldTeamID, NewTeamID); 
}

void UOWLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	BroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}
