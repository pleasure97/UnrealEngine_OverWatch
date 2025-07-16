// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/Respawn/DeathOverlay.h"
#include "UI/Widget/Respawn/RespawnGauge.h"
#include "Components/Button.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Team/OWTeamSubsystem.h"
#include "Message/OWMessageTypes.h"
#include "OWGameplayTags.h"

void UDeathOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Bind Button Clicked 
	if (Button_ChangeSpectator)
	{
		Button_ChangeSpectator->OnClicked.AddDynamic(this, &UDeathOverlay::ChangeSpectator); 
	}

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	RespawnWaitingListener = 
		GameplayMessageSubsystem.RegisterListener<FInteractionDurationInfo>(
			FOWGameplayTags::Get().Gameplay_Message_RespawnWaiting, this, &UDeathOverlay::OnRespawnWaitStarted); 

	RespawnCompletedListener =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(
			FOWGameplayTags::Get().Gameplay_Message_RespawnCompleted, this, &UDeathOverlay::OnRespawnWaitCompleted);
}

void UDeathOverlay::NativeDestruct()
{
	// Remove Binding of Button Clicked
	if (Button_ChangeSpectator)
	{
		Button_ChangeSpectator->OnClicked.RemoveAll(this); 
	}

	if (RespawnWaitingListener.IsValid())
	{
		RespawnWaitingListener.Unregister(); 
	}

	if (RespawnCompletedListener.IsValid())
	{
		RespawnCompletedListener.Unregister();
	}

	Super::NativeDestruct();
}

void UDeathOverlay::ChangeSpectator()
{
	if (IsValid(Spectator))
	{
		WatchLiveTeamMember(); 
	}
	else
	{
		BecomeSpectator(); 
		WatchLiveTeamMember(); 
	}
}

void UDeathOverlay::OnRespawnWaitStarted(FGameplayTag Channel, const FInteractionDurationInfo& Payload)
{
	if (GetOwningPlayerState() == Payload.PlayerState)
	{
		if (WBP_RespawnGauge)
		{
			WBP_RespawnGauge->InitializeRespawnGauge(Payload.InteractionDuration); 
		}
	}
}

void UDeathOverlay::OnRespawnWaitCompleted(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	if (GetOwningPlayerState() == Payload.Instigator)
	{
		if (WBP_RespawnGauge)
		{
			WBP_RespawnGauge->FinishTimer();
		}
		SetVisibility(ESlateVisibility::Collapsed); 
		
		RemoveFromParent();
	}
}

void UDeathOverlay::WatchLiveTeamMember()
{
	// Get Game State 
	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(this))
	{
		// Get Player Array from Game State
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (APawn* EachPawn = PlayerState->GetPawn())
			{
				if (EachPawn != CurrentlyFollowingTeamMember)
				{
					if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
					{
						if (TeamSubsystem->CompareTeams(EachPawn, GetOwningPlayerState()) == EOWTeamComparison::OnSameTeam)
						{
							CurrentlyFollowingTeamMember = EachPawn; 
							AttachToLiveTeamMember(CurrentlyFollowingTeamMember, Spectator);
							break;
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Currently Following Pawn in UDeathOverlay::WatchLiveTeamMember()")); 
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Cannot Get Pawn from Player State in UDeathOverlay::WatchLiveTeamMember()")); 
			}
		}

	}
}

void UDeathOverlay::BecomeSpectator()
{
	if (GetOwningPlayer())
	{
		// Unpossess Dead Hero 
		GetOwningPlayer()->UnPossess();

		// Initialize Spectator's Actor Spawn Parameters, Spawn Location, and Rotation 
		FActorSpawnParameters SpawnParameters; 
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 

		FVector SpawnLocation = FVector::ZeroVector; 
		FRotator SpawnRotation = FRotator::ZeroRotator; 

		// Spawn Spectator 
		Spectator = GetWorld()->SpawnActor<APawn>(SpectatorClass, SpawnLocation, SpawnRotation, SpawnParameters);

		// Possess Spectator 
		if (IsValid(Spectator))
		{
			GetOwningPlayer()->Possess(Spectator);
		}
	}
}

void UDeathOverlay::AttachToLiveTeamMember(APawn* TeamMember, APawn* SpectatorPawn)
{
	if (SpectatorPawn)
	{
		FDetachmentTransformRules DetachmentTransformRules 
			= FDetachmentTransformRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, true);

		SpectatorPawn->DetachFromActor(DetachmentTransformRules); 

		FAttachmentTransformRules AttachmentTransformRules
			= FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true); 
		SpectatorPawn->AttachToActor(TeamMember, AttachmentTransformRules); 
	}
}
