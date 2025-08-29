// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AssaultPoint.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Game/MatchScoringComponent.h"
#include "OWGameplayTags.h"
#include "Team/OWTeamSubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"

// Sets default values
AAssaultPoint::AAssaultPoint()
{
	bReplicates = true; 
	PrimaryActorTick.bCanEverTick = true;

	// Create Box Collision and Set Root Component to Box Collision 
	Box = CreateDefaultSubobject<UBoxComponent>("Box"); 
	Box->OnComponentBeginOverlap.AddDynamic(this, &AAssaultPoint::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AAssaultPoint::OnOverlapEnd);
	SetRootComponent(Box); 

	// Create Ability System Component and Set Replication Mode to Mixed 
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Prevent Excessive Replication of Assault Progress due to Tick()
	NetUpdateFrequency = 10.f; 
	MinNetUpdateFrequency = 5.f; 
}

UAbilitySystemComponent* AAssaultPoint::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AAssaultPoint::BeginPlay()
{
	Super::BeginPlay();

	// Get Match Scoring Component of Game State in World and Register Assault Point to Match Scoring Component
	if (UWorld* World = GetWorld())
	{
		if (AGameStateBase* GameState = UGameplayStatics::GetGameState(World))
		{
			MatchScoringComponent = GameState->GetComponentByClass<UMatchScoringComponent>();
			if (MatchScoringComponent)
			{
				MatchScoringComponent->RegisterAssaultPoint(this);
			}
		}

		if (UOWGamePhaseSubsystem* GamePhaseSubsystem = World->GetSubsystem<UOWGamePhaseSubsystem>())
		{
			// Connect to When First Team Offense Game Phase Starts
			FirstTeamOffensePhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &AAssaultPoint::HandleFirstTeamOffensePhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_MatchInProgress_FirstTeamOffense, EPhaseTagMatchType::ExactMatch, FirstTeamOffensePhaseDelegate);

			// Connect to When Switch Inning Game Phase Starts
			SwitchInningPhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &AAssaultPoint::HandleSwitchInningPhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_SwitchInning, EPhaseTagMatchType::ExactMatch, SwitchInningPhaseDelegate);

			// Connect to When Second Team Offense Game Phase Starts
			SecondTeamOffensePhaseDelegate =
				FOWGamePhaseTagDelegate::CreateUObject(this, &AAssaultPoint::HandleSecondTeamOffensePhase);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
				FOWGameplayTags::Get().GamePhase_MatchInProgress_SecondTeamOffense, EPhaseTagMatchType::ExactMatch, SecondTeamOffensePhaseDelegate);
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
	DOREPLIFETIME(AAssaultPoint, AssaultPointID); 
	DOREPLIFETIME(AAssaultPoint, OverlappingPawns);	
}

void AAssaultPoint::HandleFirstTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	bAssaultPointActive = true; 
	MeasureWhenPhaseEnds(PhaseTag, PhaseDuration);
}

void AAssaultPoint::HandleSwitchInningPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	// Change Assault Point ID like Player Start ID due to Switch Inning 
	if (AssaultPointID == 1)
	{
		AssaultPointID = 2;
	}
	else if (AssaultPointID == 2)
	{
		AssaultPointID = 1; 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Assault Point is not 1 or 2.")); 
	}

	// Reset Occupation Progress 
	OccupationProgress = 0.f; 

	bMatchDecided = false; 
}

void AAssaultPoint::HandleSecondTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	bAssaultPointActive = true;
	MeasureWhenPhaseEnds(PhaseTag, PhaseDuration);
}

void AAssaultPoint::MeasureWhenPhaseEnds(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	// Set Timer for Phase Duration and Call SendAssaultScoreWhenPhaseEnds() 
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PhaseEndTimerHandle); 

		World->GetTimerManager().SetTimer(PhaseEndTimerHandle, this, &AAssaultPoint::SendAssaultScoreWhenPhaseEnds, PhaseDuration, false);
	}
}

void AAssaultPoint::SendAssaultScoreWhenPhaseEnds()
{
	// Check if Match is Already Decided
	if (!bMatchDecided)
	{
		if (UWorld* World = GetWorld())
		{
			// Get Game Phase Subsystem
			if (UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>())
			{
				// Get GameplayTag Singleton Class
				const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
				// Check What Game Phase are Active and Send Occupation Progress to Match Scoring Component using Server RPC
				if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense))
				{
					Server_SendAssaultScore(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense, OccupationProgress);
				}
				else if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense))
				{
					Server_SendAssaultScore(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense, OccupationProgress);
				}
			}
		}
		bMatchDecided = true;
	}
}

void AAssaultPoint::Server_SendAssaultScore_Implementation(const FGameplayTag& CurrentGamePhaseTag, const float DecidedOccupationProgress)
{
	// Check if Match Scoring Component (Actor Component of Game State) is Valid 
	if (IsValid(MatchScoringComponent))
	{
		// First Team Offense Game Phase - Team 1 Occupation Progress
		if (CurrentGamePhaseTag == FOWGameplayTags::Get().GamePhase_MatchInProgress_FirstTeamOffense)
		{
			MatchScoringComponent->SetTeamOccupationProgress(1, DecidedOccupationProgress);
		}
		// Second Team Offense Game Phase - Team 2 Occupation Progress
		else if (CurrentGamePhaseTag == FOWGameplayTags::Get().GamePhase_MatchInProgress_SecondTeamOffense)
		{
			MatchScoringComponent->SetTeamOccupationProgress(2, DecidedOccupationProgress);
		}

		bAssaultPointActive = false;
	}
}

void AAssaultPoint::Tick(float DeltaSeconds)
{
	// Check Assault Point Has Authority (Server-Side)
	if (!HasAuthority() || !bAssaultPointActive)
	{
		return;
	}

	// EOccupation::Start - When there are Only Attackers at Assault Point 
	if (OccupationState == EOccupationState::Start)
	{
		// The More Attackers there are, the Faster Occupation Progress becomes 
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

		// Clamp Occupation Progress so that it does not Exceed 1
		OccupationProgress = FMath::Clamp(OccupationProgress, 0.f, 1.f);
		if (OccupationProgress >= 1.f)
		{
			if (!bMatchDecided)
			{
				OccupationState = EOccupationState::Complete;
				if (UWorld* World = GetWorld())
				{
					// Get Game Phase Subsystem
					if (UOWGamePhaseSubsystem* GamePhaseSubsystem = World->GetSubsystem<UOWGamePhaseSubsystem>())
					{
						// Get GameplayTag Singleton Class 
						const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
						// Game Phase - First Team Offense
						if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense))
						{
							Server_SendAssaultScore(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense, OccupationProgress);
						}
						// Game Phase - Second Team Offense
						else if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense))
						{
							Server_SendAssaultScore(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense, OccupationProgress);
						}
					}
				}
				bMatchDecided = true; 
			}
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
		UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
		if (GamePhaseSubsystem)
		{
			// Check Current Game Phase is "Playing"
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
			if (!GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense) &&
				!GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense))
			{
				return;
			}
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
		UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
		if (GamePhaseSubsystem)
		{
			// Check Current Game Phase is "Playing"
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
			if (!GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense) &&
				!GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense))
			{
				return;
			}
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
			// Number of Defenders > 0 / Number of Attackers > 0 
			if (NewNumAttackers > 0)
			{
				OccupationState = EOccupationState::Contesting; 
			}
			// Number of Defenders > 0 / Number of Attackers - 0 
			else
			{
				OccupationState = EOccupationState::Stop;	
			}
		}
		else
		{
			// Number of Defenders = 0 / Number of Attackers > 0 
			if (NewNumAttackers > 0)
			{
				OccupationState = EOccupationState::Start;
			}
			// Number of Defenders = 0 / Number of Attackers = 0 
			else
			{
				OccupationState = EOccupationState::Stop; 
			}
		}
	}
}

void AAssaultPoint::OnRep_NumAttackers()
{
	// Broadcast the Number of Attackers - Related to Assault Overlay 
	OnNumAttackersChanged.Broadcast(NumAttackers); 
}

void AAssaultPoint::OnRep_NumDefenders()
{
	// Broadcast the Number of Defenders - Related to Assault Overlay 
	if (NumAttackers > 0)
	{
		OnNumDefendersChanged.Broadcast(NumDefenders); 
	}
}

void AAssaultPoint::OnRep_OccupationProgress()
{
	// Broadcast the Occupation Progress - Related to Assault Overlay 
	if (FMath::Abs(OccupationProgress - LastBroadcastedOccupationProgress) > 0.01f)
	{
		LastBroadcastedOccupationProgress = OccupationProgress;
		OnOccupationProgressChanged.Broadcast(OccupationProgress); 
	}
}

void AAssaultPoint::OnRep_OccupationState()
{
	// Broadcast the Occupation State - Related to Assault Overlay 
	if (LastOccupationState == OccupationState)
	{
		return;
	}

	OnOccupationStateChanged.Broadcast(OccupationState); 
	LastOccupationState = OccupationState; 
}