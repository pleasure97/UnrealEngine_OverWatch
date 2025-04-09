// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/OWTeamSubsystem.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamInfoBase.h"
#include "Team/OWTeamPublicInfo.h"
#include "Team/OWTeamPrivateInfo.h"

void FOWTeamTrackingInfo::SetTeamInfo(AOWTeamInfoBase* Info)
{
    if (AOWTeamPublicInfo* NewPublicInfo = Cast<AOWTeamPublicInfo>(Info))
    {
        ensure((PublicInfo == nullptr) || (PublicInfo == NewPublicInfo)); 
        PublicInfo = NewPublicInfo; 

        UOWTeamDisplayAsset* OldDisplayAsset = DisplayAsset; 
        DisplayAsset = NewPublicInfo->GetTeamDisplayAsset(); 

        if (OldDisplayAsset != DisplayAsset)
        {
            // TODO 
        }
    }
    else if (AOWTeamPrivateInfo* NewPrivateInfo = Cast<AOWTeamPrivateInfo>(Info))
    {
        ensure((NewPrivateInfo == nullptr) || (PrivateInfo == NewPrivateInfo)); 
        PrivateInfo = NewPrivateInfo; 
    }
}

void FOWTeamTrackingInfo::RemoveTeamInfo(AOWTeamInfoBase* Info)
{
    if (PublicInfo == Info)
    {
        PublicInfo = nullptr; 
    }
    else if (PrivateInfo == Info)
    {
        PrivateInfo = nullptr; 
    }
}

bool UOWTeamSubsystem::RegisterTeamInfo(AOWTeamInfoBase* TeamInfo)
{
    if (!ensure(TeamInfo)) return false; 

    const int32 TeamID = TeamInfo->GetTeamID(); 
    if (ensure(TeamID != INDEX_NONE))
    {
        FOWTeamTrackingInfo& Entry = TeamMap.FindOrAdd(TeamID); 
        Entry.SetTeamInfo(TeamInfo); 

        return true; 
    }
    return false;
}

bool UOWTeamSubsystem::UnregisterTeamInfo(AOWTeamInfoBase* TeamInfo)
{
    if (!ensure(TeamInfo)) return false; 

    const int32 TeamID = TeamInfo->GetTeamID(); 
    if (ensure(TeamID != INDEX_NONE))
    {
        FOWTeamTrackingInfo* Entry = TeamMap.Find(TeamID); 
        if (Entry)
        {
            Entry->RemoveTeamInfo(TeamInfo); 
            return true;
        }
    }
    return false; 
}

const AOWPlayerState* UOWTeamSubsystem::FindPlayerStateFromActor(const AActor* PossibleTeamActor) const
{
    if (PossibleTeamActor != nullptr)
    {
        if (const APawn* Pawn = Cast<const APawn>(PossibleTeamActor))
        {
            if (AOWPlayerState* OWPlayerState = Pawn->GetPlayerState<AOWPlayerState>())
            {
                return OWPlayerState;
            }
        }
        else if (const AController* Controller = Cast<const AController>(PossibleTeamActor))
        {
            if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(Controller->PlayerState))
            {
                return OWPlayerState;
            }
        }
        else if (const AOWPlayerState* OWPlayerState = Cast<const AOWPlayerState>(PossibleTeamActor))
        {
            return OWPlayerState;
        }
    }

    return nullptr;
}

int32 UOWTeamSubsystem::FindTeamFromObject(const UObject* TestObject) const
{
    if (const ITeamInterface* ObjectWithTeamInterface = Cast<ITeamInterface>(TestObject))
    {
        return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
    }

    if (const AActor* TestActor = Cast<const AActor>(TestObject))
    {
        if (const ITeamInterface* InstigatorWithTeamInterface = Cast<ITeamInterface>(TestActor->GetInstigator()))
        {
            return GenericTeamIdToInteger(InstigatorWithTeamInterface->GetGenericTeamId()); 
        }
        if (const AOWTeamInfoBase* TeamInfo = Cast<AOWTeamInfoBase>(TestActor))
        {
            return TeamInfo->GetTeamID(); 
        }
        if (const AOWPlayerState* OWPlayerState = FindPlayerStateFromActor(TestActor))
        {
            return OWPlayerState->GetTeamId(); 
        }
    }

    return INDEX_NONE; 
}

EOWTeamComparison UOWTeamSubsystem::CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const
{
    TeamIdA = FindTeamFromObject(Cast<const AActor>(A)); 
    TeamIdB = FindTeamFromObject(Cast<const AActor>(B)); 

    if ((TeamIdA == INDEX_NONE) || (TeamIdB == INDEX_NONE))
    {
        return EOWTeamComparison::InvalidArgument; 
    }
    else
    {
        return (TeamIdA == TeamIdB) ? EOWTeamComparison::OnSameTeam : EOWTeamComparison::DifferentTeams; 
    }
}

EOWTeamComparison UOWTeamSubsystem::CompareTeams(const UObject* A, const UObject* B) const
{
    int32 TeamIdA; 
    int32 TeamIdB; 
    return CompareTeams(A, B, TeamIdA, TeamIdB); 
}

bool UOWTeamSubsystem::CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf) const
{
    if (bAllowDamageToSelf)
    {
        if ((Instigator == Target) || (FindPlayerStateFromActor(Cast<AActor>(Instigator)) == FindPlayerStateFromActor(Cast<AActor>(Target))))
        {
            return true; 
        }
    }

    int32 InstigatorTeamID; 
    int32 TargetTeamID; 

    const EOWTeamComparison Relationship = CompareTeams(Instigator, Target, InstigatorTeamID, TargetTeamID);
    if (Relationship == EOWTeamComparison::DifferentTeams)
    {
        return true; 
    }
 
    return false;
}