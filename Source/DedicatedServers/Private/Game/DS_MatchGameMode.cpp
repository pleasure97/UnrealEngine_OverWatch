// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_MatchGameMode.h"
#include "Player/DSPlayerController.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/GameStats/GameStatsManager.h"

ADS_MatchGameMode::ADS_MatchGameMode()
{
	bUseSeamlessTravel = true; 
	MatchStatus = EMatchStatus::WaitingForPlayers; 

	FirstHeroSelectionTimer.Type = ECountTimerType::FirstHeroSelection; 
	FirstMatchPreparationTimer.Type = ECountTimerType::FirstMatchPreparation;
	FirstMatchTimer.Type = ECountTimerType::FirstMatch; 
	SwitchInningTimer.Type = ECountTimerType::SwitchInning;
	SecondHeroSelectionTimer.Type = ECountTimerType::SecondHeroSelection;
	SecondMatchPreparationTimer.Type = ECountTimerType::SecondMatchPreparation;
	SecondMatchTimer.Type = ECountTimerType::SecondMatch;
	PostMatchTimer.Type = ECountTimerType::PostMatch; 
}

void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer); 

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::FirstHeroSelection;
		StartCountTimer(FirstHeroSelectionTimer);
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
		MatchStatus = EMatchStatus::FirstHeroSelection;
		StartCountTimer(FirstHeroSelectionTimer);
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

	switch (Type)
	{
	case ECountTimerType::FirstHeroSelection:
	{
		StopCountTimer(FirstHeroSelectionTimer);
		MatchStatus = EMatchStatus::FirstMatchPreparation;
		StartCountTimer(FirstMatchPreparationTimer);
		return;
	}
	case ECountTimerType::FirstMatchPreparation:
	{
		StopCountTimer(FirstMatchPreparationTimer);
		MatchStatus = EMatchStatus::FirstMatch;
		StartCountTimer(FirstMatchTimer);
		return;
	}
	case ECountTimerType::FirstMatch:
	{
		StopCountTimer(FirstMatchTimer);
		MatchStatus = EMatchStatus::SwitchInning;
		StartCountTimer(SwitchInningTimer);
		return;
	}
	case ECountTimerType::SwitchInning:
	{
		StopCountTimer(SwitchInningTimer);
		MatchStatus = EMatchStatus::SecondHeroSelection;
		StartCountTimer(SecondHeroSelectionTimer);
		return;
	}
	case ECountTimerType::SecondHeroSelection:
	{
		StopCountTimer(SecondHeroSelectionTimer);
		MatchStatus = EMatchStatus::SecondMatchPreparation;
		StartCountTimer(SecondMatchPreparationTimer);
		return;
	}
	case ECountTimerType::SecondMatchPreparation:
	{
		StopCountTimer(SecondMatchPreparationTimer);
		MatchStatus = EMatchStatus::SecondMatch;
		StartCountTimer(SecondMatchTimer);
		return;
	}
	case ECountTimerType::SecondMatch:
	{
		StopCountTimer(SecondMatchTimer);
		MatchStatus = EMatchStatus::PostMatch;
		StartCountTimer(PostMatchTimer);
		SetClientInputEnabled(false);
		OnMatchEnded();
		return;
	}
	case ECountTimerType::PostMatch:
	{
		StopCountTimer(PostMatchTimer);
		MatchStatus = EMatchStatus::SeamlessTravelling;
		TrySeamlessTravel(LobbyMap);
		return;
	}
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
