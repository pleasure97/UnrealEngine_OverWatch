// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AssaultPoint.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Game/MatchScoringComponent.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "OWGameplayTags.h"
#include "Team/OWTeamSubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"

// Sets default values
AAssaultPoint::AAssaultPoint()
{
	bReplicates = true; 
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box"); 
	Box->OnComponentBeginOverlap.AddDynamic(this, &AAssaultPoint::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AAssaultPoint::OnOverlapEnd);
	SetRootComponent(Box); 

	// Prevent Excessive Replication of Assault Progress due to Tick()
	NetUpdateFrequency = 10.f; 
	MinNetUpdateFrequency = 5.f; 
}

// Called when the game starts or when spawned
void AAssaultPoint::BeginPlay()
{
	Super::BeginPlay();

	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(this))
	{
		MatchScoringComponent = GameState->GetComponentByClass<UMatchScoringComponent>();
		if (MatchScoringComponent)
		{
			MatchScoringComponent->RegisterAssaultPoint(this); 
		}
	}
}

void AAssaultPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(AAssaultPoint, NumAttackers);
	DOREPLIFETIME(AAssaultPoint, NumDefenders);
	DOREPLIFETIME(AAssaultPoint, OccupationProgress); 
	DOREPLIFETIME(AAssaultPoint, OccupationState); 

	DOREPLIFETIME(AAssaultPoint, OverlappingPawns);	
}

void AAssaultPoint::Tick(float DeltaSeconds)
{
	if (!HasAuthority())
	{
		return;
	}

	if (OccupationState == EOccupationState::Start)
	{
		if (NumAttackers >= 3)
		{
			OccupationProgress += BaseCaptureRate * ThreePeopleMultiplier * DeltaSeconds; 
		}
		else if (NumAttackers == 2)
		{
			OccupationProgress += BaseCaptureRate * TwoPeopleMultiplier * DeltaSeconds;

		}
		else if (NumAttackers == 1)
		{
			OccupationProgress += BaseCaptureRate * DeltaSeconds; 
		}

		OccupationProgress = FMath::Clamp(OccupationProgress, 0.f, 1.f);
		if (OccupationProgress >= 1.f)
		{
			OccupationState = EOccupationState::Complete; 
			// TODO - Update Match Scoring Component 
		}
	}

	AccumulatedDebugTime += DeltaSeconds; 
	if (AccumulatedDebugTime >= Interval)
	{
		AccumulatedDebugTime = 0.f; 

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Num Attackers = %d"), NumAttackers));
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Num Defenders = %d"), NumDefenders));
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Occupation Progress = %f"), OccupationProgress));
		}
	}
}

void AAssaultPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check Has Authority
	if (HasAuthority())
	{
		// Get Team Subsystem 
		UOWTeamSubsystem* OWTeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
		if (!OWTeamSubsystem)
		{
			return;
		}

		// Get Game Phase Subsystem 
		UOWGamePhaseSubsystem* OWGamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
		if (OWGamePhaseSubsystem)
		{
			//// Check Current Game Phase is "Playing"
			//bool PlayingPhaseActive = OWGamePhaseSubsystem->IsPhaseActive(FOWGameplayTags::Get().GamePhase_MatchInProgress);
			//if (!PlayingPhaseActive)
			//{
			//	return;
			//}
		}

		// Cast Actor to Pawn
		if (APawn* OverlappingPawn = Cast<APawn>(OtherActor))
		{
			// Save Casted Pawn to Member Variable Array 
			OverlappingPawns.AddUnique(OverlappingPawn);

			// Find Overlapping Pawn's Team ID 
			int32 OverlappingPawnTeamID = OWTeamSubsystem->FindTeamFromObject(OverlappingPawn);

			// Early Return if Overlapping Pawn's Team ID is not Initialized. 
			if (OverlappingPawnTeamID <= 0)
			{
				return;
			}

			// Distinguish whether Overlapping Pawn is Attacker or Defender
			if (OverlappingPawnTeamID == AssaultPointID)
			{
				UpdateAssaultPoint(NumAttackers, NumDefenders + 1); 
			}
			else
			{
				UpdateAssaultPoint(NumAttackers + 1, NumDefenders);
			}
		}

	}
}

void AAssaultPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	// Check Has Authority 
	if (HasAuthority())
	{
		// Get Team Subsystem 
		UOWTeamSubsystem* OWTeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
		if (!OWTeamSubsystem)
		{
			return;
		}

		// Get Game Phase Subsystem 
		UOWGamePhaseSubsystem* OWGamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
		if (OWGamePhaseSubsystem)
		{
			//// Check Current Game Phase is "Playing"
			//bool PlayingPhaseActive = OWGamePhaseSubsystem->IsPhaseActive(FOWGameplayTags::Get().GamePhase_MatchInProgress);
			//if (!PlayingPhaseActive)
			//{
			//	return;
			//}
		}

		// Cast Actor to Pawn 
		// Remove Overlapping Pawn from Overlapping Pawns Array 
		if (APawn* OverlappingPawn = Cast<APawn>(OtherActor))
		{
			OverlappingPawns.Remove(OverlappingPawn); 

			// Find Overlapping Pawn's Team ID 
			int32 OverlappingPawnTeamID = OWTeamSubsystem->FindTeamFromObject(OverlappingPawn);

			// Early Return if Overlapping Pawn's Team ID is not Initialized. 
			if (OverlappingPawnTeamID <= 0)
			{
				return;
			}

			// Distinguish whether Overlapping Pawn is Attacker or Defender
			if (OverlappingPawnTeamID == AssaultPointID)
			{
				UpdateAssaultPoint(NumAttackers, NumDefenders - 1);
			}
			else
			{
				UpdateAssaultPoint(NumAttackers - 1, NumDefenders);
			}
		}
	}
}

void AAssaultPoint::UpdateAssaultPoint(int32 NewNumAttackers, int32 NewNumDefenders)
{
	// Early Return if Occupation State is Complete 
	if (OccupationState == EOccupationState::Complete)
	{
		return;
	}

	// Check Has Authority 
	if (HasAuthority())
	{
		int32 OldNumAttackers = NumAttackers; 
		int32 OldNumDefenders = NumDefenders; 

		NumAttackers = NewNumAttackers; 
		NumDefenders = NewNumDefenders; 

		if (NewNumDefenders > 0)
		{
			if (NewNumAttackers > 0)
			{
				OccupationState = EOccupationState::Contesting; 
			}
			else
			{
				OccupationState = EOccupationState::Stop;	
			}
		}
		// The Number of New Defenders is 0 
		else
		{
			if (NewNumAttackers > 0)
			{
				OccupationState = EOccupationState::Start;
			}
			else
			{
				OccupationState = EOccupationState::Stop; 
			}
		}
	}
}

void AAssaultPoint::OnRep_NumAttackers()
{
	OnNumAttackersChanged.Broadcast(NumAttackers); 
}

void AAssaultPoint::OnRep_NumDefenders()
{
	if (NumAttackers > 0)
	{
		OnNumDefendersChanged.Broadcast(NumDefenders); 
	}
}

void AAssaultPoint::OnRep_OccupationProgress()
{
	if (FMath::Abs(OccupationProgress - LastBroadcastedOccupationProgress) > 0.01f)
	{
		LastBroadcastedOccupationProgress = OccupationProgress;
		OnOccupationProgressChanged.Broadcast(OccupationProgress); 
	}
}

void AAssaultPoint::OnRep_OccupationState()
{
	if (LastOccupationState == OccupationState)
	{
		return;
	}

	OnOccupationStateChanged.Broadcast(OccupationState); 
	LastOccupationState = OccupationState; 
}

void AAssaultPoint::MakeOccupationMessage()
{
	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);

	// Initialize Occupation Info Message 
	FOccupationInfo OccupationInfo;
	OccupationInfo.NumAttackers = NumAttackers;
	OccupationInfo.NumDefenders = NumDefenders;
	OccupationInfo.OccupationState = OccupationState;
	OccupationInfo.OccupationProgress = OccupationProgress;

	// Broadcast Occupation Info Message
	GameplayMessageSubsystem.BroadcastMessage(FOWGameplayTags::Get().Gameplay_Message_AssaultProgress, OccupationInfo);
}
