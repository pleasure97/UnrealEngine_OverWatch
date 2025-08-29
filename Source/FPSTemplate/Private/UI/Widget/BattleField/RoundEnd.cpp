// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BattleField/RoundEnd.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "Game/MatchScoringComponent.h"
#include "Components/TextBlock.h"

void URoundEnd::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Get Match Scoring Component of Game State from World 
	if (UWorld* World = GetWorld())
	{
		if (AGameStateBase* GameState = UGameplayStatics::GetGameState(World))
		{
			if (UMatchScoringComponent* MatchScoringComponent = GameState->FindComponentByClass<UMatchScoringComponent>())
			{
				// Get Team1 Assault Score, Occupation Progress, and Remaining Time from Match Scoring Component 
				int32 Team1Score = MatchScoringComponent->GetTeamScore(1); 
				// Team1 Assault Score (0 - MAX)
				if (TextBlock_Team1Score)
				{
					FString Team1ScoreString = FString::FromInt(Team1Score); 
					TextBlock_Team1Score->SetText(FText::FromString(Team1ScoreString));
				}
				// Team1 Assault Occupation Progress (0.00 - 100.00)
				float Team1Progress = MatchScoringComponent->GetTeamOccupationProgress(1); 
				if (TextBlock_Team1OccupationProgress)
				{
					const float Team1ProgressPercent = Team1Progress * 100.f;
					const FString Team1ProgressString = FString::Printf(TEXT("%.2f"), Team1ProgressPercent);
					TextBlock_Team1OccupationProgress->SetText(FText::FromString(Team1ProgressString));
				}
				// Team1 Remaining Time (4:00 - 0:00)
				float Team1RemainingTime = MatchScoringComponent->GetTeamRemainingTime(1); 
				if (TextBlock_Team1RemainingTime)
				{
					FText Team1RemainingTimeText = ConvertTimeText(Team1RemainingTime); 
					TextBlock_Team1RemainingTime->SetText(Team1RemainingTimeText); 
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Cannot Find Match Scoring Component in URoundEnd::NativeConstruct()."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot Get Game State in URoundEnd::NativeConstruct().")); 
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Get World in URoundEnd::NativeConstruct().")); 
	}

	// Play Round End Animation 
	if (RoundEndAnimation)
	{
		PlayAnimation(RoundEndAnimation); 
	}
}

FText URoundEnd::ConvertTimeText(float RemainingCountdownTime)
{
	int32 TotalSeconds = FMath::FloorToInt(RemainingCountdownTime);
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	FString SecondsString = (Seconds < 10) ? FString::Printf(TEXT("0%d"), Seconds) : FString::FromInt(Seconds);
	FString TimeString = FString::Printf(TEXT("%d:%s"), Minutes, *SecondsString);

	return FText::FromString(TimeString);
}
