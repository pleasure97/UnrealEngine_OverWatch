// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/OWTeamInfoBase.h"
#include "Net/UnrealNetwork.h"
#include "Team/OWTeamSubsystem.h"

AOWTeamInfoBase::AOWTeamInfoBase()
{
	bReplicates = true; 
	bAlwaysRelevant = true; 
	NetPriority = 3.f; 
	SetReplicatingMovement(false); 
}

void AOWTeamInfoBase::BeginPlay()
{
	Super::BeginPlay(); 

	TryRegisterWithTeamSubsystem(); 
}

void AOWTeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason); 
}

void AOWTeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(AOWTeamInfoBase, TeamTag); 
	DOREPLIFETIME_CONDITION(AOWTeamInfoBase, TeamID, COND_InitialOnly); 
}

void AOWTeamInfoBase::RegisterWithTeamSubsystem(UOWTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this); 
}

void AOWTeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamID != -1)
	{
		UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
		if (TeamSubsystem)
		{
			RegisterWithTeamSubsystem(TeamSubsystem); 
		}
	}
}

void AOWTeamInfoBase::OnRep_TeamID()
{
	TryRegisterWithTeamSubsystem(); 
}

void AOWTeamInfoBase::SetTeamID(int32 NewTeamID)
{
	if (!HasAuthority()) return; 
	if (TeamID != -1 || NewTeamID == -1) return; 

	TeamID = NewTeamID; 

	TryRegisterWithTeamSubsystem(); 
}
