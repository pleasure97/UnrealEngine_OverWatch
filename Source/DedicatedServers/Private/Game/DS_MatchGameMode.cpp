// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_MatchGameMode.h"
#include "Player/DSPlayerController.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/GameStats/GameStatsManager.h"

ADS_MatchGameMode::ADS_MatchGameMode()
{
	bUseSeamlessTravel = true; 
	MatchStatus = EMatchStatus::WaitingForPlayers; 
	PreMatchTimer.Type = ECountTimerType::PreMatch; 
	MatchTimer.Type = ECountTimerType::Match; 
	PostMatchTimer.Type = ECountTimerType::PostMatch; 
}

void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer); 

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch; 
		StartCountTimer(PreMatchTimer); 
	}
}

void ADS_MatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting); 

	RemovePlayerSession(Exiting); 
}

void ADS_MatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController); 

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountTimer(PreMatchTimer);
	}
}

void ADS_MatchGameMode::BeginPlay()
{
	Super::BeginPlay(); 

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass); 
	GameStatsManager->OnUpdateLeaderboardSucceeded.AddDynamic(this, &ADS_MatchGameMode::OnLeaderboardUpdated); 
}

void ADS_MatchGameMode::OnCountTimerFinished(ECountTimerType Type)
{
	Super::OnCountTimerFinished(Type); 

	if (Type == ECountTimerType::PreMatch)
	{
		StopCountTimer(PreMatchTimer); 
		MatchStatus = EMatchStatus::Match; 
		StartCountTimer(MatchTimer); 
		SetClientInputEnabled(true); 
	}
	if (Type == ECountTimerType::Match)
	{
		StopCountTimer(MatchTimer);
		MatchStatus = EMatchStatus::PostMatch; 
		StartCountTimer(PostMatchTimer); 
		SetClientInputEnabled(false); 
		OnMatchEnded(); 
	}
	if (Type == ECountTimerType::PostMatch)
	{
		StopCountTimer(PostMatchTimer);
		MatchStatus = EMatchStatus::SeamlessTravelling; 
		TrySeamlessTravel(LobbyMap); 
	}
}

void ADS_MatchGameMode::SetClientInputEnabled(bool bEnabled)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get()); 
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_SetInputEnabled(bEnabled); 
		}
	}
}

void ADS_MatchGameMode::OnMatchEnded()
{
	EndMatchForPlayerStates();
}

void ADS_MatchGameMode::EndMatchForPlayerStates()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get()); IsValid(DSPlayerController))
		{
			if (ADS_MatchPlayerState* MatchPlayerState = DSPlayerController->GetPlayerState<ADS_MatchPlayerState>(); IsValid(MatchPlayerState))
			{
				MatchPlayerState->OnMatchEnded(DSPlayerController->Username);
			}
		}

	}
}

void ADS_MatchGameMode::UpdateLeaderboard(const TArray<FString>& LeaderboardNames)
{
	if (IsValid(GameStatsManager))
	{
		GameStatsManager->UpdateLeaderboard(LeaderboardNames); 
	}
}

void ADS_MatchGameMode::OnLeaderboardUpdated()
{
	EndMatchForPlayerStates(); 
}
