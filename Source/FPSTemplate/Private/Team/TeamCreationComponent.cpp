// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/TeamCreationComponent.h"
#include "GameFramework/GameState.h"
#include "Player/OWPlayerState.h"
#include "Data/OWTeamDisplayAsset.h"
#include "Team/OWTeamInfoBase.h"
#include "Game/OWGameModeBase.h"

void UTeamCreationComponent::BeginPlay()
{
	Super::BeginPlay(); 

	AGameState* GameState = CastChecked<AGameState>(GetOwner()); 

#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateTeams(); 
		ServerAssignPlayersToTeams(); 
	}
#endif 
}

bool UTeamCreationComponent::HasAuthority() const
{
	AActor* Owner = GetOwner(); 
	check(Owner); 
	return Owner->HasAuthority(); 
}

#if WITH_SERVER_CODE
void UTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& Team : TeamsToCreate)
	{
		const int32 TeamID = Team.Key; 
		ServerCreateTeam(TeamID, Team.Value); 
	}
}

void UTeamCreationComponent::ServerAssignPlayersToTeams()
{
	// Assign Players that already exist to teams 
	AGameState* GameState = CastChecked<AGameState>(GetOwner());
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
		{
			ServerChooseTeamForPlayer(OWPlayerState); 
		}
	}

	// Listen for New Players Logging In 
	AOWGameModeBase* OWGameModeBase = Cast<AOWGameModeBase>(GameState->AuthorityGameMode); 
	check(OWGameModeBase); 

	OWGameModeBase->OnGameModePlayerInitialized.AddUObject(this, &UTeamCreationComponent::OnPlayerInitialized); 
}

void UTeamCreationComponent::OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState); 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(NewPlayer->PlayerState))
	{
		ServerChooseTeamForPlayer(OWPlayerState); 
	}
}

void UTeamCreationComponent::ServerChooseTeamForPlayer(AOWPlayerState* OWPlayerState)
{
	if (OWPlayerState->IsOnlyASpectator())
	{
		OWPlayerState->SetGenericTeamId(FGenericTeamId::NoTeam); 
	}
	else
	{
		const FGenericTeamId TeamID = IntegerToGenericTeamId(GetLeastPopulatedTeamID()); 
		OWPlayerState->SetGenericTeamId(TeamID); 
	}
}

void UTeamCreationComponent::ServerCreateTeam(int32 TeamID, UOWTeamDisplayAsset* DisplayAsset)
{
	check(HasAuthority());

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AOWTeamInfoBase* NewTeamPublicInfo = World->SpawnActor<AOWTeamInfoBase>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"), *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamID(TeamID);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset); 

	AOWTeamInfoBase* NewTeamPrivateInfo = World->SpawnActor<AOWTeamInfoBase>(PrivateTeamInfoClass, SpawnInfo); 
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"), *GetPathNameSafe(*PrivateTeamInfoClass)); 
	NewTeamPrivateInfo->SetTeamID(TeamID); 
}

int32 UTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num(); 
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts; 
		TeamMemberCounts.Reserve(NumTeams); 

		for (const auto& Team : TeamsToCreate)
		{
			const int32 TeamID = Team.Key; 
			TeamMemberCounts.Add(TeamID, 0); 
		}

		AGameState* GameState = CastChecked<AGameState>(GetOwner());
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
			{
				const int32 PlayerTeamID = OWPlayerState->GetTeamId(); 
				if ((PlayerTeamID != INDEX_NONE) && !OWPlayerState->IsInactive())
				{
					check(TeamMemberCounts.Contains(PlayerTeamID)); 
					TeamMemberCounts[PlayerTeamID] += 1; 
				}
			}
		}

		int32 BestTeamId = INDEX_NONE; 
		uint32 BestTeamPlayerCount = TNumericLimits<uint32>::Max(); 
		for (const auto& TeamMemberCount : TeamMemberCounts)
		{
			const int32 TestTeamId = TeamMemberCount.Key; 
			const uint32 TestTeamPlayerCount = TeamMemberCount.Value; 
			if (TestTeamPlayerCount < BestTeamPlayerCount)
			{
				BestTeamId = TestTeamId; 
				BestTeamPlayerCount = TestTeamPlayerCount; 
			}
			else if (TestTeamPlayerCount == BestTeamPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId; 
					BestTeamPlayerCount = TestTeamPlayerCount; 
				}
			}
		}

		return BestTeamId; 
	}
	return INDEX_NONE; 
}
#endif 