// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameModeBase.h"
#include "Game/OWGameState.h"
#include "Character/OWCharacter.h"
#include "Player/OWPlayerState.h"
#include "Game/PlayerSpawningManagerComponent.h"
#include "Team/OWTeamSubsystem.h"

void AOWGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Cast<APlayerController>(*Iterator); 
		if ((PlayerController != nullptr) && (PlayerController->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PlayerController))
			{
				RestartPlayer(PlayerController); 
			}
		}
	}
}

void AOWGameModeBase::ChangeHero(APlayerController* PlayerController, EHeroName NewHeroName)
{
	if (!PlayerController) return; 

	AOWCharacter* CurrentHero = Cast<AOWCharacter>(PlayerController->GetPawn()); 

	const FVector CurrentHeroLocation = CurrentHero->GetActorLocation(); 
	const FRotator CurrentHeroRotation = CurrentHero->GetActorRotation(); 

	if (CurrentHero)
	{
		CurrentHero->Destroy(); 
	}

	TSubclassOf<AOWCharacter>* NewHeroClass = HeroMap.Find(NewHeroName); 
	if (NewHeroClass && *NewHeroClass)
	{
		FActorSpawnParameters SpawnParams; 
		SpawnParams.Owner = PlayerController; 
		SpawnParams.Instigator = PlayerController->GetPawn(); 

		AOWCharacter* NewCharacter = GetWorld()->SpawnActor<AOWCharacter>(*NewHeroClass, 
			CurrentHeroLocation, CurrentHeroRotation, SpawnParams); 
		if (NewCharacter)
		{
			PlayerController->Possess(NewCharacter); 
		}
	}

	if (AOWPlayerState* OWPlayerState = PlayerController->GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->SetHeroName(NewHeroName);
	}
}

//bool AOWGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
//{
//	return false;
//}

void AOWGameModeBase::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer); 

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer); 
}

AActor* AOWGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (UPlayerSpawningManagerComponent* PlayerSpawningManagerComponent = GameState->FindComponentByClass<UPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningManagerComponent->ChoosePlayerStart(Player); 
	}

	return Super::ChoosePlayerStart_Implementation(Player); 
}

void AOWGameModeBase::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	if (UPlayerSpawningManagerComponent* PlayerSpawningManagerComponent = GameState->FindComponentByClass<UPlayerSpawningManagerComponent>())
	{
		PlayerSpawningManagerComponent->FinishRestartPlayer(NewPlayer, StartRotation); 
	}

	Super::FinishRestartPlayer(NewPlayer, StartRotation); 
}

bool AOWGameModeBase::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player); 
}

void AOWGameModeBase::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer); 

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APlayerController* NewPlayerController = Cast<APlayerController>(NewPlayer))
		{
			if (PlayerCanRestart(NewPlayerController))
			{
				RequestPlayerRestartNextFrame(NewPlayer); 
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going to try again."), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer);
		}
	}
}

void AOWGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (NewPlayer && NewPlayer->PlayerState)
	{
		// TODO - Parse Options, or Load from Lobby Data 
		/*const int32 ChosenTeamID = 1; 

		UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
		if (TeamSubsystem)
		{
			if (!TeamSubsystem->ChangeTeamForActor(NewPlayer, ChosenTeamID))
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to Change Team for Actor: Controller = % s, TeamID = % d"),
					*NewPlayer->GetName(), ChosenTeamID);
			}
		}*/
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

bool AOWGameModeBase::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return true;
}

bool AOWGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AOWGameModeBase::RequestPlayerRestartNextFrame(AController* Controller)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PlayerController, &APlayerController::ServerRestartPlayer_Implementation); 
	}
}

bool AOWGameModeBase::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (!Super::PlayerCanRestart_Implementation(PlayerController))
		{
			return false;
		}
	}
	
	if (UPlayerSpawningManagerComponent* PlayerSpawningManagerComponent = GameState->FindComponentByClass<UPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningManagerComponent->ControllerCanRestart(Controller); 
	}

	return true;
}

void AOWGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{

}
