// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameModeBase.h"
#include "Game/OWGameState.h"
#include "Character/OWCharacter.h"
#include "Player/OWPlayerState.h"
#include "Player/OWPlayerController.h"
#include "Player/OWPlayerStart.h"
#include "Game/PlayerSpawningManagerComponent.h"
#include "Team/OWTeamSubsystem.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

void AOWGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 

	if (UWorld* World = GetWorld())
	{
		if (UOWGamePhaseSubsystem* GamePhaseSubsystem = World->GetSubsystem<UOWGamePhaseSubsystem>())
		{
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

			FirstHeroSelectionEndedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleFirstHeroSelectionPhase);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, EPhaseTagMatchType::ExactMatch, FirstHeroSelectionEndedDelegate);
		
			SwitchInningEndedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleSwitchInningPhase);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_SwitchInning, EPhaseTagMatchType::ExactMatch, SwitchInningEndedDelegate);
			
			SecondHeroSelectionEndedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleSecondHeroSelectionPhase);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_HeroSelection_SecondHeroSelection, EPhaseTagMatchType::ExactMatch, SecondHeroSelectionEndedDelegate);
		}
	}
}

void AOWGameModeBase::HandleFirstHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	EnableHeroSpawning(); 
}

void AOWGameModeBase::HandleSwitchInningPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	CleanupOldPawns(); 
}

void AOWGameModeBase::HandleSecondHeroSelectionPhase(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	EnableHeroSpawning(); 
}

void AOWGameModeBase::EnableHeroSpawning()
{
	if (UWorld* World = GetWorld())
	{
		bCanSpawnHero = true;

		UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(GetWorld());

		for (APlayerController* PendingPlayerController : PendingPlayers)
		{
			if (PendingPlayerController && (!PendingPlayerController->GetPawn()))
			{
				if (AOWPlayerState* OWPlayerState = PendingPlayerController->GetPlayerState<AOWPlayerState>())
				{
					if (OWPlayerState->GetHeroName() != EHeroName::None)
					{
						FOWHeroInfo HeroInfoToSpawn = HeroInfo->HeroInformation[OWPlayerState->GetHeroName()];
						RestartHeroWithClass(PendingPlayerController, HeroInfoToSpawn.HeroPawnClass);
					}
				}
			}
		}
		PendingPlayers.Empty();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Get World from Game Mode Base.")); 
	}
}

void AOWGameModeBase::RestartHeroWithClass(APlayerController* PC, TSubclassOf<APawn> PawnClass)
{
	if (!PC || !PawnClass)
	{
		return;
	}

	AActor* StartSpot = ChoosePlayerStart(PC); 
	if (!StartSpot)
	{
		UE_LOG(LogTemp, Error, TEXT("There is No Valid Player Start in AOWGameModeBase::RestartHeroWithClass()")); 
		return;
	}

	AOWPlayerStart* OWPlayerStart = Cast<AOWPlayerStart>(StartSpot); 
	if (!OWPlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Cast to OWPlayerStart in AOWGameModeBase::RestartHeroWithClass()"));
		return;
	}

	if (!OWPlayerStart->TryClaim(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Already Claimed!"), *OWPlayerStart->GetName()); 
	}

	FTransform SpawnTransform = OWPlayerStart->GetActorTransform(); 
	FActorSpawnParameters SpawnParameters; 
	SpawnParameters.Owner = PC; 
	SpawnParameters.Instigator = PC->GetPawn() ? PC->GetPawn()->GetInstigator() : nullptr; 
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 
	SpawnParameters.bNoFail = true; 
	SpawnParameters.bDeferConstruction = false; 

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnParameters); 
	if (NewPawn)
	{
		PC->Possess(NewPawn); 
	}
		
	FinishRestartPlayer(PC, SpawnTransform.GetRotation().Rotator()); 
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

	if (AOWPlayerState* OWPlayerState = PlayerController->GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->SetHeroName(NewHeroName);
	}
}

void AOWGameModeBase::CleanupOldPawns()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(*Iterator))
		{
			if (APawn* OldPawn = PlayerController->GetPawn())
			{
				PlayerController->UnPossess(); 

				OldPawn->Destroy(); 
			}
		}
	}
}

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
	if (bCanSpawnHero)
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
	else
	{
		PendingPlayers.Add(NewPlayer); 
	}
}

void AOWGameModeBase::StartPlay()
{
	Super::StartPlay(); 

	OnGameplayReady.Broadcast(); 
}

void AOWGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer); 

	if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(NewPlayer))
	{
		FString Username = OWPlayerController->Username; 
		if (!Username.IsEmpty())
		{
			if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(OWPlayerController->PlayerState))
			{
				OWPlayerState->SetPlayerName(Username);
			}
		}
	}
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