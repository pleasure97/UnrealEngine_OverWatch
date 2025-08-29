// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BattleField/MatchDecided.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "OWGameplayTags.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Game/MatchScoringComponent.h"

void UMatchDecided::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Cast Owning Player State to Custom Player State 
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetOwningPlayerState()))
	{
		// Get World
		if (UWorld* World = GetWorld())
		{
			// Get Team Subsystem
			if (UOWTeamSubsystem* TeamSubsystem = World->GetSubsystem<UOWTeamSubsystem>())
			{
				// Get Owner Team ID 
				OwnerTeamID = TeamSubsystem->FindTeamFromObject(OWPlayerState);
				if ((OwnerTeamID != 1) && (OwnerTeamID != 2))
				{
					UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not 1 or 2 in UMatchDecided::NativeConstruct()")); 
					return;
				}
				
				// Get Game State
				if (AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(World))
				{
					// Get Match Scoring Component which is Actor Component of Game State
					if (UMatchScoringComponent* MatchScoringComponent = GameStateBase->GetComponentByClass<UMatchScoringComponent>())
					{
						// Get GameplayTag Singleton Class
						const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

						// Check Current Game Phase Tag (Replicated) of Match Scoring Component and Update according to Current Game Phase Tag 
						// Current Game Phase Tag = First Team Offense (Match in Progress)
						if (MatchScoringComponent->CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_FirstTeamOffense)
						{
							if (TextBlock_MatchDecided)
							{
								TextBlock_MatchDecided->SetText(RoundEndText);
								TextBlock_MatchDecided->SetColorAndOpacity(VictoryTextColor);
							}
						}
						// Current Game Phase Tag = Second Team Offense (Match in Progress)
						else if (MatchScoringComponent->CurrentGamePhaseTag == GameplayTags.GamePhase_MatchInProgress_SecondTeamOffense)
						{
							MatchScoringComponent->OnWinningTeamDecided.AddDynamic(this, &UMatchDecided::OnWinningTeamDecided); 

							if (MatchScoringComponent->GetWinningTeamID() < 0)
							{
								UE_LOG(LogTemp, Error, TEXT("Winning Team ID is Not Set in UMatchDecided::NativeConstruct()"));
								return; 
							}

							// Check if Owner Wins
							OnWinningTeamDecided(MatchScoringComponent->GetWinningTeamID());
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("MatchDecided Widget is Created at Invalid Time")); 
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Cannot Get Team Subsystem from World in UMatchDecided::NativeConstruct()")); 
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot Get World in UMatchDecided::NativeConstruct()"));

		}
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Owning Player State is not Valid or Cannot Cast to Custom Player State in UMatchDecided::NativeConstruct()")); 
	}

	// Play Match Decided Animation 
	if (MatchDecidedAnimation)
	{
		PlayAnimation(MatchDecidedAnimation);
	}
}

void UMatchDecided::OnWinningTeamDecided(int32 WinningTeamID)
{
	// Owner Team - Victory 
	if (WinningTeamID == OwnerTeamID)
	{
		if (TextBlock_MatchDecided)
		{
			TextBlock_MatchDecided->SetText(VictoryText);
			TextBlock_MatchDecided->SetColorAndOpacity(VictoryTextColor);
		}
	}
	// Owner Team - Defeated 
	else
	{
		if (TextBlock_MatchDecided)
		{
			TextBlock_MatchDecided->SetText(DefeatedText);
			TextBlock_MatchDecided->SetColorAndOpacity(DefeatedTextColor);
		}
	}
}
