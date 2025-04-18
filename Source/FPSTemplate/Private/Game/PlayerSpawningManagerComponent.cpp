// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PlayerSpawningManagerComponent.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/OWGameState.h"
#include "Player/OWPlayerStart.h"

UPlayerSpawningManagerComponent::UPlayerSpawningManagerComponent()
{
	// Not Replicated on Network 
	SetIsReplicatedByDefault(false);
	// This Component is auto registered and activated in owner. 
	bAutoRegister = true; 
	bAutoActivate = true; 
	// When Game Starts, InitializeComponents() is Called. 
	bWantsInitializeComponent = true; 
	// Primary Component Tick Settings
	PrimaryComponentTick.TickGroup = TG_PrePhysics; 
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true; 
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerSpawningManagerComponent::InitializeComponent()
{
	Super::InitializeComponent(); 

	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UPlayerSpawningManagerComponent::OnLevelAdded); 

	UWorld* World = GetWorld(); 
	World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &UPlayerSpawningManagerComponent::HandleOnActorSpawned));

	for (TActorIterator<AOWPlayerStart> It(World); It; ++It)
	{
		if (AOWPlayerStart* PlayerStart = *It)
		{
			CachedPlayerStarts.Add(PlayerStart); 
		}
	}
}

AActor* UPlayerSpawningManagerComponent::ChoosePlayerStart(AController* Player)
{
	if (Player)
	{
#if WITH_EDITOR
		if (AOWPlayerStart* PlayerStart = FindPlayFromHereStart(Player))
		{
			return PlayerStart; 
		}
#endif

		TArray<AOWPlayerStart*> StarterPoints; 
		for (auto StartIterator = CachedPlayerStarts.CreateIterator(); StartIterator; ++StartIterator)
		{
			if (AOWPlayerStart* Start = (*StartIterator).Get())
			{
				StarterPoints.Add(Start); 
			}
			else
			{
				StartIterator.RemoveCurrent(); 
			}
		}

		AActor* PlayerStart = OnChoosePlayerStart(Player, StarterPoints); 

		if (!PlayerStart)
		{
			PlayerStart = GetFirstRandomUnoccupiedPlayerStart(Player, StarterPoints); 
		}

		if (AOWPlayerStart* OWPlayerStart = Cast<AOWPlayerStart>(PlayerStart))
		{
			OWPlayerStart->TryClaim(Player); 
		}

		return PlayerStart; 
	}
	return nullptr;
}

AActor* UPlayerSpawningManagerComponent::OnChoosePlayerStart(AController* Player, TArray<AOWPlayerStart*>& PlayerStarts)
{
	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
	if (!ensure(TeamSubsystem)) { return nullptr; }

	const int32 PlayerTeamID = TeamSubsystem->FindTeamFromObject(Player);
	if (!ensure(PlayerTeamID != INDEX_NONE)) { return nullptr; }

	AOWGameState* GameState = CastChecked<AOWGameState>(GetOwner());

	AOWPlayerStart* BestPlayerStart = nullptr;
	double MaxDistance = 0;
	AOWPlayerStart* FallbackPlayerStart = nullptr;
	double FallbackMaxDistance = 0;

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		const int32 TeamID = TeamSubsystem->FindTeamFromObject(PlayerState);

		if (!ensure(TeamID != INDEX_NONE)) { continue; }

		if (TeamID != PlayerTeamID)
		{
			for (AOWPlayerStart* PlayerStart : PlayerStarts)
			{
				if (APawn* Pawn = PlayerState->GetPawn())
				{
					const double Distance = PlayerStart->GetDistanceTo(Pawn);

					// TODO
				}
			}
		}
	}

	if (BestPlayerStart)
	{
		return BestPlayerStart;
	}

	return FallbackPlayerStart;
}

AOWPlayerStart* UPlayerSpawningManagerComponent::GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AOWPlayerStart*>& FoundStartPoints) const
{
	if (Controller)
	{
		TArray<AOWPlayerStart*> UnoccupiedStartPoints;
		TArray<AOWPlayerStart*> OccupiedStartPoints;

		for (AOWPlayerStart* FoundStartPoint : FoundStartPoints)
		{
			EOWPlayerStartOccupancy State = FoundStartPoint->GetLocationOccupancy(Controller);
			switch (State)
			{
			case EOWPlayerStartOccupancy::Empty:
				UnoccupiedStartPoints.Add(FoundStartPoint);
				break;
			case EOWPlayerStartOccupancy::Partial:
				OccupiedStartPoints.Add(FoundStartPoint);
				break;
			}
		}

		if (UnoccupiedStartPoints.Num() > 0)
		{
			return UnoccupiedStartPoints[FMath::RandRange(0, UnoccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}
	return nullptr;
}

bool UPlayerSpawningManagerComponent::ControllerCanRestart(AController* Player)
{
	bool bCanRestart = true; 

	return bCanRestart;
}

void UPlayerSpawningManagerComponent::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	OnFinishRestartPlayer(NewPlayer, StartRotation);
}

void UPlayerSpawningManagerComponent::OnLevelAdded(ULevel* InLevel, UWorld* InWorld)
{
	if (InWorld == GetWorld())
	{
		for (AActor* Actor : InLevel->Actors)
		{
			if (AOWPlayerStart* PlayerStart = Cast<AOWPlayerStart>(Actor))
			{
				CachedPlayerStarts.Add(PlayerStart); 
			}
		}
	}
}

void UPlayerSpawningManagerComponent::HandleOnActorSpawned(AActor* SpawnedActor)
{
	if (AOWPlayerStart* PlayerStart = Cast<AOWPlayerStart>(SpawnedActor))
	{
		CachedPlayerStarts.Add(PlayerStart); 
	}
}

# if WITH_EDITOR
AOWPlayerStart* UPlayerSpawningManagerComponent::FindPlayFromHereStart(AController* Player)
{
	if (Player->IsA<APlayerController>())
	{
		if (UWorld* World = GetWorld())
		{
			for (TActorIterator<AOWPlayerStart> It(World); It; ++It)
			{
				if (AOWPlayerStart* PlayerStart = *It)
				{
					if (PlayerStart->IsA<APlayerStartPIE>())
					{
						return PlayerStart; 
					}
				}
			}
		}
	}
	return nullptr; 
}
# endif


void UPlayerSpawningManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

