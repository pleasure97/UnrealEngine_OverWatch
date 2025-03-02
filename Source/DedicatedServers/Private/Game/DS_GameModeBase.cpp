// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameModeBase.h"
#include "Player/DSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameLiftServerSDK.h"

void ADS_GameModeBase::StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.TimerFinishedDelegate.BindWeakLambda(this,
		[&]()
		{
			OnCountdownTimerFinished(CountdownTimerHandle.Type); 
		});

	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerFinishedHandle, 
		CountdownTimerHandle.TimerFinishedDelegate, 
		CountdownTimerHandle.CountdownTime,
		false); 

	CountdownTimerHandle.TimerUpdateDelegate.BindWeakLambda(this,
		[&]()
		{
			UpdateCountdownTimer(CountdownTimerHandle); 
		});

	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerUpdateHandle,
		CountdownTimerHandle.TimerUpdateDelegate,
		CountdownTimerHandle.CountdownUpdateInterval,
		true);
	UpdateCountdownTimer(CountdownTimerHandle);
}

void ADS_GameModeBase::StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.State = ECountdownTimerState::Stopped;
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerFinishedHandle); 
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerUpdateHandle);
	if (CountdownTimerHandle.TimerFinishedDelegate.IsBound())
	{
		CountdownTimerHandle.TimerFinishedDelegate.Unbind(); 
	}
	if (CountdownTimerHandle.TimerUpdateDelegate.IsBound())
	{
		CountdownTimerHandle.TimerUpdateDelegate.Unbind(); 
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get()); 
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_TimerStopped(0.f, CountdownTimerHandle.Type); 
		}
	}
}

void ADS_GameModeBase::UpdateCountdownTimer(const FCountdownTimerHandle& CountdownTimerHandle)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
		if (IsValid(DSPlayerController))
		{
			const float CountdownTimeLeft = CountdownTimerHandle.CountdownTime - GetWorldTimerManager().GetTimerElapsed(CountdownTimerHandle.TimerFinishedHandle);
			DSPlayerController->Client_TimerUpdated(CountdownTimeLeft, CountdownTimerHandle.Type);
		}
	}
}

void ADS_GameModeBase::OnCountdownTimerFinished(ECountdownTimerType Type)
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
