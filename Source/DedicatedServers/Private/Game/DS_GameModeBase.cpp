// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameModeBase.h"
#include "Player/DSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameLiftServerSDK.h"

void ADS_GameModeBase::StartCountTimer(FCountTimerHandle& CountTimerHandle)
{
	// Bind Timer Finshed Delegate when Timer Direction is Countdown 
	if (CountTimerHandle.Direction == ECountTimerDirection::Countdown)
	{
		CountTimerHandle.TimerFinishedDelegate.BindWeakLambda(this,
			[&]()
			{
				OnCountTimerFinished(CountTimerHandle.Type);
			});

		GetWorldTimerManager().SetTimer(
			CountTimerHandle.TimerFinishedHandle,
			CountTimerHandle.TimerFinishedDelegate,
			CountTimerHandle.CountTime,
			false);
	}
	
	CountTimerHandle.TimerUpdateDelegate.BindWeakLambda(this,
		[&]()
		{
			UpdateCountTimer(CountTimerHandle); 
		});

	GetWorldTimerManager().SetTimer(
		CountTimerHandle.TimerUpdateHandle,
		CountTimerHandle.TimerUpdateDelegate,
		CountTimerHandle.CountUpdateInterval,
		true);

	UpdateCountTimer(CountTimerHandle);
}

void ADS_GameModeBase::StopCountTimer(FCountTimerHandle& CountTimerHandle)
{	
	CountTimerHandle.State = ECountTimerState::Stopped;
	GetWorldTimerManager().ClearTimer(CountTimerHandle.TimerFinishedHandle); 
	GetWorldTimerManager().ClearTimer(CountTimerHandle.TimerUpdateHandle);
	if (CountTimerHandle.TimerFinishedDelegate.IsBound())
	{
		CountTimerHandle.TimerFinishedDelegate.Unbind(); 
	}
	if (CountTimerHandle.TimerUpdateDelegate.IsBound())
	{
		CountTimerHandle.TimerUpdateDelegate.Unbind(); 
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get()); 
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_TimerStopped(0.f, CountTimerHandle.Direction, CountTimerHandle.Type); 
		}
	}
}

void ADS_GameModeBase::UpdateCountTimer(const FCountTimerHandle& CountTimerHandle)
{
	// Iterate Player Controller
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		// Cast to DSPlayerController 
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
		if (IsValid(DSPlayerController))
		{
			float CountTime = 0.f; 

			// Depend on Countup or Countdown 
			switch (CountTimerHandle.Direction)
			{
			case(ECountTimerDirection::Countup):
			{
				// Calculate Time using Timer Finshed Handle's Elapsed Time 
				CountTime = CountTimerHandle.CountTime - GetWorldTimerManager().GetTimerElapsed(CountTimerHandle.TimerFinishedHandle);
				break;
			}
			case (ECountTimerDirection::Countdown):
			{
				// Calculate Time using Timer Update Handle's Elapsed Time 
				CountTime = GetWorldTimerManager().GetTimerElapsed(CountTimerHandle.TimerUpdateHandle); 
				break;
			}
			}

			DSPlayerController->Client_TimerUpdated(CountTime, CountTimerHandle.Direction, CountTimerHandle.Type);
		}
	}
}

void ADS_GameModeBase::OnCountTimerFinished(ECountTimerType Type)
{
}

void ADS_GameModeBase::TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap)
{
	const FString MapName = DestinationMap.ToSoftObjectPath().GetAssetName();
	if (GIsEditor)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
	else
	{
		GetWorld()->ServerTravel(MapName);
	}
}

void ADS_GameModeBase::RemovePlayerSession(AController* Exiting)
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Exiting);
	if (!IsValid(DSPlayerController)) return;

#if WITH_GAMELIFT
	const FString& PlayerSessionId = DSPlayerController->PlayerSessionId;
	if (!PlayerSessionId.IsEmpty())
	{
		Aws::GameLift::Server::RemovePlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
	}
#endif 
}
