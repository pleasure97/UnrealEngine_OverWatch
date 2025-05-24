// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/AsyncAction_ObserveTeam.h"
#include "Interface/TeamInterface.h"

UAsyncAction_ObserveTeam* UAsyncAction_ObserveTeam::ObserveTeam(UObject* TeamAgent)
{
	return InternalObserveTeamChanges(TeamAgent); 
}

UAsyncAction_ObserveTeam* UAsyncAction_ObserveTeam::InternalObserveTeamChanges(TScriptInterface<ITeamInterface> TeamActor)
{
	UAsyncAction_ObserveTeam* Action = nullptr;

	if (TeamActor != nullptr)
	{
		Action = NewObject<UAsyncAction_ObserveTeam>();
		Action->TeamInterfacePtr = TeamActor;
		Action->RegisterWithGameInstance(TeamActor.GetObject());
	}

	return Action; 
}

void UAsyncAction_ObserveTeam::Activate()
{
	// Initialize bool variable to check succeed and team index 
	bool bCouldSucceed = false;
	int32 CurrentTeamIndex = INDEX_NONE; 

	if (ITeamInterface* TeamInterface = TeamInterfacePtr.Get())
	{
		CurrentTeamIndex = GenericTeamIdToInteger(TeamInterface->GetGenericTeamId()); 
		TeamInterface->GetTeamChangedDelegate().AddDynamic(this, &UAsyncAction_ObserveTeam::OnWatchedAgentChangedTeam); 
		bCouldSucceed = true; 
	}

	OnTeamChanged.Broadcast(CurrentTeamIndex != INDEX_NONE, CurrentTeamIndex); 

	if (!bCouldSucceed)
	{
		SetReadyToDestroy();
	}
}

void UAsyncAction_ObserveTeam::SetReadyToDestroy()
{
	Super::SetReadyToDestroy(); 

	if (ITeamInterface* TeamInterface = TeamInterfacePtr.Get())
	{
		TeamInterface->GetTeamChangedDelegate().RemoveAll(this); 
	}
}

void UAsyncAction_ObserveTeam::OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	OnTeamChanged.Broadcast(NewTeam != INDEX_NONE, NewTeam); 
}


