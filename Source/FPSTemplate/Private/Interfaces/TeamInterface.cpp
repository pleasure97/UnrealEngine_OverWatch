// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/TeamInterface.h"


// Add default functionality here for any ITeamInterface functions that are not pure virtual.

void ITeamInterface::BroadcastTeamChanged(TScriptInterface<ITeamInterface> TeamInterface, FGenericTeamId OldTeamID, FGenericTeamId NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID); 
		const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID); 

		UObject* TeamObject = TeamInterface.GetObject(); 

		TeamInterface.GetInterface()->GetTeamChangedDelegate().Broadcast(TeamObject, OldTeamIndex, NewTeamIndex);
	}
}
