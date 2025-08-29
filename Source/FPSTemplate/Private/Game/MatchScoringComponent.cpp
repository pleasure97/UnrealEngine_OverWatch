// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MatchScoringComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "Actor/AssaultPoint.h"
#include "Character/OWCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "OWGameplayTags.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/OWGameState.h"
#include "Player/OWPlayerStart.h"
#include "Game/OWGameModeBase.h"
#include "AbilitySystem/OWGlobalAbilitySystem.h"


UMatchScoringComponent::UMatchScoringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); 
}

void UMatchScoringComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMatchScoringComponent, AssaultPoints);
	DOREPLIFETIME(UMatchScoringComponent, CountdownTime);
	DOREPLIFETIME(UMatchScoringComponent, Team1OccupationProgress);
	DOREPLIFETIME(UMatchScoringComponent, Team2OccupationProgress);
	DOREPLIFETIME(UMatchScoringComponent, Team1AssaultScore);
	DOREPLIFETIME(UMatchScoringComponent, Team2AssaultScore);
	DOREPLIFETIME(UMatchScoringComponent, Team1RemainingTime);
	DOREPLIFETIME(UMatchScoringComponent, Team2RemainingTime);
	DOREPLIFETIME(UMatchScoringComponent, CurrentGamePhaseTag);
	DOREPLIFETIME(UMatchScoringComponent, WinningTeamID);
}

void UMatchScoringComponent::BeginPlay()
{
	Super::BeginPlay();

	// Delay BeginPlay() Until Player States are Properly Initialized
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayTimerHandle); 
		World->GetTimerManager().SetTimer(
			DelayTimerHandle,
			this,
			&UMatchScoringComponent::ConnectToGamePhase,
			3.f,
			false);
	}
}

void UMatchScoringComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear Timer Handle - Delay Timer Handle, Countdown Timer Handle 
	// TODO - Delete Delay Timer Handle and Make sure the Loading Screen is Visible until the Game is Set Up
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayTimerHandle); 
		World->GetTimerManager().ClearTimer(CountdownTimerHandle); 
		World->GetTimerManager().ClearTimer(GameplayEffectTimerHandle);
	}

	// Unbind Delegates Related to When Game Phase Starts or Is Active 
	if (FirstTeamOffensePhaseDelegate.IsBound())
	{
		FirstTeamOffensePhaseDelegate.Unbind();
	}

	if (FirstMatchPreparationDelegate.IsBound())
	{
		FirstMatchPreparationDelegate.Unbind();
	}

	if (FirstTeamOffensePhaseDelegate.IsBound())
	{
		FirstTeamOffensePhaseDelegate.Unbind();
	}

	if (SecondHeroSelectionGamePhaseDelegate.IsBound())
	{
		SecondHeroSelectionGamePhaseDelegate.Unbind();
	}

	if (SecondMatchPreparationDelegate.IsBound())
	{
		SecondMatchPreparationDelegate.Unbind();
	}

	if (SecondTeamOffensePhaseDelegate.IsBound())
	{
		SecondTeamOffensePhaseDelegate.Unbind();
	}

	Super::EndPlay(EndPlayReason); 
}

void UMatchScoringComponent::RegisterAssaultPoint(AAssaultPoint* AssaultPoint)
{
	AssaultPoints.Add(AssaultPoint);
	OnAssaultPointRegistered.Broadcast(AssaultPoint); 
}

AAssaultPoint* UMatchScoringComponent::GetActiveAssaultPoint() const
{
	for (AAssaultPoint* RegisteredAssaultPoint : AssaultPoints)
	{
		if (RegisteredAssaultPoint->IsAssaultPointActive())
		{
			return RegisteredAssaultPoint; 
		}
	}
	return nullptr;
}

void UMatchScoringComponent::OnRep_AssaultPoints()
{
	for (AAssaultPoint* AssaultPoint : AssaultPoints)
	{
		OnAssaultPointRegistered.Broadcast(AssaultPoint);
	}
}

void UMatchScoringComponent::OnRep_CountdownTime()
{
	OnPhaseRemainingTime.Broadcast(CurrentGamePhaseTag, CountdownTime); 
}

void UMatchScoringComponent::OnRep_Team1OccupationProgress()
{
	// Prevent RepNotify from Being Called Again After Match Has been Already Decided 
	if (!bFirstTeamOffenseMatchDecided)
	{
		bFirstTeamOffenseMatchDecided = true;

		// Get GameplayTag Singleton Container
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

		// Check if Current Game Phase is 'First Team Offense' 
		if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense)
		{
			// Record Team1 Remaining Time 
			Team1RemainingTime = CountdownTime;
		}

		OnOccupationProgressDecided.Broadcast(CurrentGamePhaseTag, Team1OccupationProgress);

		// Proceed to Prepare for Next Game Phase 
		// Check Has Authority, and Get Game Phase Subsystem and Global Ability System 
		if (GetOwner()->HasAuthority())
		{
			if (UWorld* World = GetWorld())
			{
				if (UOWGamePhaseSubsystem* GamePhaseSubsystem = World->GetSubsystem<UOWGamePhaseSubsystem>())
				{
					if (GamePhaseSubsystem->IsPhaseActive(FOWGameplayTags::Get().GamePhase_MatchInProgress_FirstTeamOffense))
					{
						if (UOWGlobalAbilitySystem* GlobalAbilitySystem = World->GetSubsystem<UOWGlobalAbilitySystem>())
						{
							// Apply Match Decided Gameplay Effect
							GlobalAbilitySystem->ApplyEffectToAll(MatchDecidedGameplayEffect);
							World->GetTimerManager().ClearTimer(GameplayEffectTimerHandle); 
							// Prepare Next Game Phase 
							World->GetTimerManager().SetTimer(
								GameplayEffectTimerHandle, 
								this, 
								&UMatchScoringComponent::PrepareNextGamePhase,
								5.f, 
								false); 
						}
					}
				}
			}
		}
	}
}

void UMatchScoringComponent::OnRep_Team1AssaultScore()
{
	OnAssaultScoreDecided.Broadcast(1, Team1AssaultScore); 
}

void UMatchScoringComponent::OnRep_Team2OccupationProgress()
{
	// Prevent RepNotify from Being Called Again After Match Has been Already Decided 
	if (!bSecondTeamOffenseMatchDecided)
	{
		bSecondTeamOffenseMatchDecided = true;

		// Get GameplayTag Singleton Container
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

		// Check if Current Game Phase is 'Second Team Offense' 
		if (CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
		{
			// Record Team2 Remaining Time 
			Team2RemainingTime = CountdownTime; 
		}

		// TODO - Gameplay Cue 
		OnOccupationProgressDecided.Broadcast(CurrentGamePhaseTag, Team2OccupationProgress);


		// Proceed to Prepare for Next Game Phase 
		// Decide Which Team Wins 
		if (GetOwner()->HasAuthority())
		{
			// Get Game Phase Subsystem and Check if Game Phase is 'Second Team Offense' 
			if (UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>())
			{
				if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense))
				{
					// Victory Conditions - Assault Score, Occupation Progress in order
					if (Team1AssaultScore > Team2AssaultScore)
					{
						HandleVictory(1);
					}
					else if (Team1AssaultScore < Team2AssaultScore)
					{
						HandleVictory(2);
					}
					else
					{
						if (Team1OccupationProgress > Team2OccupationProgress)
						{
							HandleVictory(1);
						}
						else if (Team1OccupationProgress < Team2OccupationProgress)
						{
							HandleVictory(2);
						}
						else
						{
							// TODO - Additional Game Phase 
							return; 
						}
					}

					// Start Post Match Game Phase After Timer Ends 
					GamePhaseSubsystem->StartPhase(PostMatchGamePhaseAbility);
				}
			}
		}
	}
}

void UMatchScoringComponent::OnRep_Team2AssaultScore()
{
	OnAssaultScoreDecided.Broadcast(2, Team2AssaultScore);
}

void UMatchScoringComponent::OnRep_WinningTeamID()
{
	OnWinningTeamDecided.Broadcast(WinningTeamID); 
}

void UMatchScoringComponent::ConnectToGamePhase()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayTimerHandle);
	}

	if (UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>())
	{
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

		if (GetOwner()->HasAuthority())
		{
			// Start First Hero Selection Game Phase
			GamePhaseSubsystem->StartPhase(FirstHeroSelectionGamePhaseAbility);
		}
		
		// Connect to When First Hero Selection Game Phase Starts
		FirstHeroSelectionGamePhaseDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleFirstHeroSelectionPhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_HeroSelection_FirstHeroSelection, EPhaseTagMatchType::ExactMatch, FirstHeroSelectionGamePhaseDelegate);

		// Connect to When First Match Preparation Game Phase Starts
		FirstMatchPreparationDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleFirstMatchPreparationPhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_MatchPreparation_FirstTeamOffense, EPhaseTagMatchType::ExactMatch, FirstMatchPreparationDelegate);

		// Connect to When First Team Offense Game Phase Starts
		FirstTeamOffensePhaseDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleFirstTeamOffensePhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_MatchInProgress_FirstTeamOffense, EPhaseTagMatchType::ExactMatch, FirstTeamOffensePhaseDelegate);

		// Connect to When Switch Inning Game Phase Starts
		SwitchInningPhaseDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSwitchInningPhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_SwitchInning, EPhaseTagMatchType::ExactMatch, SwitchInningPhaseDelegate);

		// Connect to When Second Hero Selection Game Phase Starts
		SecondHeroSelectionGamePhaseDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSecondHeroSelectionPhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_HeroSelection_SecondHeroSelection, EPhaseTagMatchType::ExactMatch, SecondHeroSelectionGamePhaseDelegate);

		// Connect to When Second Match Preparation Game Phase Starts
		SecondMatchPreparationDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSecondMatchPreparationPhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_MatchPreparation_SecondTeamOffense, EPhaseTagMatchType::ExactMatch, SecondMatchPreparationDelegate);

		// Connect to When Second Team Offense Game Phase Starts
		SecondTeamOffensePhaseDelegate =
			FOWGamePhaseTagDelegate::CreateUObject(this, &UMatchScoringComponent::HandleSecondTeamOffensePhase);
		GamePhaseSubsystem->WhenPhaseStartsOrIsActive(
			FOWGameplayTags::Get().GamePhase_MatchInProgress_SecondTeamOffense, EPhaseTagMatchType::ExactMatch, SecondTeamOffensePhaseDelegate);
	}
}

void UMatchScoringComponent::Countdown()
{
	--CountdownTime; 

	if (AOWGameState* OWGameState = Cast<AOWGameState>(GetOwner()))
	{
		FOWVerbMessage OWVerbMessage; 
		OWVerbMessage.Verb = CurrentGamePhaseTag; 
		OWVerbMessage.Magnitude = CountdownTime;
		OWGameState->MulticastReliableMessageToClients(OWVerbMessage); 
	}

	if (CountdownTime <= 0.f)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(CountdownTimerHandle); 
		}
	}
}

void UMatchScoringComponent::GamePhaseStarted(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	CurrentGamePhaseTag = PhaseTag;
	CountdownTime = PhaseDuration;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UMatchScoringComponent::Countdown, 1.f, true);
	}
}

void UMatchScoringComponent::HandleFirstHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleFirstMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleFirstTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleSwitchInningPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	TArray<AOWPlayerStart*> OWPlayerStarts = GetAllPlayerStarts();

	// Change Team ID of Each Player Start (Team ID of Player Start is Replicated)
	for (AOWPlayerStart* OWPlayerStart : OWPlayerStarts)
	{
		if (OWPlayerStart->GetTeamID() == 1)
		{
			OWPlayerStart->SetTeamID(2);
		}
		else if (OWPlayerStart->GetTeamID() == 2)
		{
			OWPlayerStart->SetTeamID(1); 
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Team ID of Player Start is not 1 or 2 in UMatchScoringComponent::HandleSwitchInningPhase()")); 
		}
	}
}

TArray<AOWPlayerStart*> UMatchScoringComponent::GetAllPlayerStarts() const
{
	// Declare Player Start Array
	TArray<AOWPlayerStart*> OWPlayerStarts;

	// Get Player Start Actors from World 
	if (UWorld* World = GetWorld())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, AOWPlayerStart::StaticClass(), FoundActors);

		// Cast Player Start to Custom Player Start and Add it to Player Start Array 
		for (AActor* FoundActor : FoundActors)
		{
			if (AOWPlayerStart* OWPlayerStart = Cast<AOWPlayerStart>(FoundActor))
			{
				OWPlayerStarts.Add(OWPlayerStart);
			}
		}
	}
	
	return OWPlayerStarts; 
}

void UMatchScoringComponent::HandleSecondHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleSecondMatchPreparationPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::HandleSecondTeamOffensePhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	GamePhaseStarted(PhaseTag, PhaseDuration);
}

void UMatchScoringComponent::PrepareNextGamePhase()
{
	// Get Global Ability System and Game Phase Subsystem from World 
	if (UWorld* World = GetWorld())
	{
		// Remove Match Decided Gameplay Effect from All Clients 
		if (UOWGlobalAbilitySystem* GlobalAbilitySystem = World->GetSubsystem<UOWGlobalAbilitySystem>())
		{
			GlobalAbilitySystem->RemoveEffectFromAll(MatchDecidedGameplayEffect); 
		}

		if (UOWGamePhaseSubsystem* GamePhaseSubsystem = World->GetSubsystem<UOWGamePhaseSubsystem>())
		{
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
			// Start 'Switch Inning' Game Phase or 'Post Match' Game Phase 
			if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense))
			{
				if (GetOwner()->HasAuthority())
				{
					GamePhaseSubsystem->StartPhase(SwitchInningGamePhaseAbility);
				}
			}
			else if (GamePhaseSubsystem->IsPhaseActive(GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense))
			{
				if (GetOwner()->HasAuthority())
				{
					GamePhaseSubsystem->StartPhase(PostMatchGamePhaseAbility);
				}
			}
		}
		
		World->GetTimerManager().ClearTimer(GameplayEffectTimerHandle);
	}
}


int32 UMatchScoringComponent::GetTeamScore(int32 TeamId) const
{
	if (TeamId == 1)
	{
		return Team1AssaultScore; 
	}
	else if (TeamId == 2)
	{
		return Team2AssaultScore; 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Team ID is not 1 or 2 in UMatchScoringComponent::GetTeamScore()")); 
		return 0;
	}
}

float UMatchScoringComponent::GetTeamOccupationProgress(int32 TeamId) const
{
	if (TeamId == 1)
	{
		return Team1OccupationProgress;
	}
	else if (TeamId == 2)
	{
		return Team2OccupationProgress;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TeamID is not 1 or 2.")); 
		return 0.0f;
	}
}

void UMatchScoringComponent::SetTeamOccupationProgress(int32 TeamId, float OccupationProgress)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (TeamId == 1)
	{
		Team1OccupationProgress = OccupationProgress;
		if (Team1OccupationProgress >= 1.f)
		{
			++Team1AssaultScore;
		}
		OnRep_Team1OccupationProgress(); 
	}
	else if (TeamId == 2)
	{
		Team2OccupationProgress = OccupationProgress;
		if (Team2OccupationProgress >= 1.f)
		{
			++Team2AssaultScore;
		}
		OnRep_Team2OccupationProgress(); 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Team ID is not 1 or 2.")); 
	}
}

float UMatchScoringComponent::GetTeamRemainingTime(int32 TeamId) const
{
	if (TeamId == 1)
	{
		return Team1RemainingTime; 
	}
	else if (TeamId == 2)
	{
		return Team2RemainingTime; 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Team ID is not 1 or 2 in MatchScoringComponent::GetRemainingTime()")); 
		return 0.0f;
	}
}

int32 UMatchScoringComponent::GetWinningTeamID() const
{
	return WinningTeamID;
}

void UMatchScoringComponent::ScoreAssaultPoint(FOccupationInfo OccupationInfo)
{
	if ((OccupationInfo.OccupationTeamID != 1) && (OccupationInfo.OccupationTeamID != 2))
	{
		return;
	}

	if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		if (OccupationInfo.OccupationProgress >= 1.f)
		{
			TeamSubsystem->AddTeamTagStack(OccupationInfo.OccupationTeamID, FOWGameplayTags::Get().BattleField_Assault_TeamScore, 1); 
		}
	}
}

void UMatchScoringComponent::HandleVictory(int32 TeamID)
{
	// Set Winning Team 
	WinningTeamID = TeamID; 
	
	// Replace Ability Level of the GameplayCueParameters with Team ID. 
	FGameplayCueParameters GameplayCueParameters; 
	GameplayCueParameters.AbilityLevel = WinningTeamID;
	ActivateMatchDecidedGameplayCue(FOWGameplayTags::Get().GameplayCue_MatchDecided, GameplayCueParameters);

	OnRep_WinningTeamID(); 
}

void UMatchScoringComponent::ActivateMatchDecidedGameplayCue(FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters)
{
	ClearMatchDecidedGameplayCue(); 

	// Check if GameplayCueTag is Valid 
	if (GameplayCueTag.IsValid())
	{
		MatchDecidedTag = GameplayCueTag;
		// Add Match Decided Gameplay Cue to Gameplay State using Gameplay Cue Function Library 
		if (AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(this))
		{
			UGameplayCueFunctionLibrary::AddGameplayCueOnActor(GameStateBase, MatchDecidedTag, GameplayCueParameters);
			for (AAssaultPoint* RegisteredAssaultPoint : AssaultPoints)
			{
				UGameplayCueFunctionLibrary::AddGameplayCueOnActor(RegisteredAssaultPoint, MatchDecidedTag, GameplayCueParameters);
			}
		}
	}
}

void UMatchScoringComponent::ClearMatchDecidedGameplayCue()
{
	// Check if Match Decided Tag Member Variable is Valid 
	if (MatchDecidedTag.IsValid())
	{
		// Remove Match Decided Gameplay Cue to Gameplay State using Gameplay Cue Function Library 
		if (AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(this))
		{
			UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(GameStateBase, MatchDecidedTag, FGameplayCueParameters()); 
			// Reset Match Decided Tag that is the Member Variable 
			MatchDecidedTag = FGameplayTag(); 
		}
	}
}