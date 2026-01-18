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
#include "UI/HUD/OWHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

void AOWGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 

	if (UWorld* World = GetWorld())
	{
		if (UOWGamePhaseSubsystem* GamePhaseSubsystem = World->GetSubsystem<UOWGamePhaseSubsystem>())
		{
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

			FirstHeroSelectionStartedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenFirstHeroSelectionEnds);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, EPhaseTagMatchType::ExactMatch, FirstHeroSelectionStartedDelegate);
		
			SwitchInningStartedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenSwitchInningStarts);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(GameplayTags.GamePhase_SwitchInning, EPhaseTagMatchType::ExactMatch, SwitchInningStartedDelegate);

			SwitchInningEndedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenSwitchInningEnds);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_SwitchInning, EPhaseTagMatchType::ExactMatch, SwitchInningEndedDelegate);
			
			SecondHeroSelectionStartedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenSecondHeroSelectionStarts);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(GameplayTags.GamePhase_HeroSelection_SecondHeroSelection, EPhaseTagMatchType::ExactMatch, SecondHeroSelectionStartedDelegate);

			SecondHeroSelectionEndedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenSecondHeroSelectionEnds);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_HeroSelection_SecondHeroSelection, EPhaseTagMatchType::ExactMatch, SecondHeroSelectionEndedDelegate);
		
			UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::BeginPlay()")); 
		}

		InitializeHeroPool();
	}
}

void AOWGameModeBase::InitializeHeroPool()
{
	TArray<AActor*> FoundActors; 

	// Find 'OWCharacter' Actors that has been Pre-Spawned in Level 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOWCharacter::StaticClass(), FoundActors); 

	// Iterate Found Actors Array and Register them to Hero Pool
	for (AActor* FoundActor : FoundActors)
	{
		// Cast Found Actor to 'OWCharacter'
		if (AOWCharacter* Hero = Cast<AOWCharacter>(FoundActor))
		{
			// Add Related Hero to Hero Pool 
			EHeroName HeroName = Hero->GetHeroName();
			if (HeroName != EHeroName::None)
			{
				// Setup Hero Pool Unit 
				FHeroPoolUnit NewUnit;
				NewUnit.HeroPawn = Hero;
				NewUnit.HeroTransform = Hero->GetActorTransform();

				HeroPool.FindOrAdd(HeroName).Heroes.Add(NewUnit);
			}

			DeactivateHero(Hero);
		}
	}

}

void AOWGameModeBase::DeactivateHero(AOWCharacter* Hero)
{
	// Hide Hero and Disable Collision and Actor Tick for Optimization
	Hero->SetActorHiddenInGame(true);
	Hero->SetActorEnableCollision(false);
	Hero->SetActorTickEnabled(false);
}

void AOWGameModeBase::HandleWhenFirstHeroSelectionEnds(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::HandleWhenFirstHeroSelectionEnds()"));
	EnableHeroSpawning();
}

void AOWGameModeBase::HandleWhenSwitchInningStarts(const FGameplayTag& PhaseTag, const float PhaseDuration)
{

}

void AOWGameModeBase::HandleWhenSwitchInningEnds(const FGameplayTag& PhaseTag, const float PhaseDuration)
{

}

void AOWGameModeBase::HandleWhenSecondHeroSelectionStarts(const FGameplayTag& PhaseTag, const float PhaseDuration)
{

}

void AOWGameModeBase::HandleWhenSecondHeroSelectionEnds(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	PushHeroesToHeroPool(); 
	EnableHeroSpawning();
}

void AOWGameModeBase::EnableHeroSpawning()
{
	if (UWorld* World = GetWorld())
	{
		bCanSpawnHero = true;

		UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(GetWorld());

		UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::EnableHeroSpawning()"));

		for (APlayerController* PendingPlayerController : PendingPlayers)
		{
			RestartHero(PendingPlayerController, HeroInfo);
		}
		PendingPlayers.Empty();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Get World from Game Mode Base.")); 
	}
}

void AOWGameModeBase::RestartHero(APlayerController* PlayerControllerToRestart, UHeroInfo* HeroInfo)
{
	UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::RestartHero()"));

	if (!HeroInfo)
	{
		HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(GetWorld());
	}

	if (PlayerControllerToRestart && (!PlayerControllerToRestart->GetPawn()))
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController %s in AOWGameModeBase::RestartHero()"), *GetPathNameSafe(PlayerControllerToRestart));

		if (AOWPlayerState* OWPlayerState = PlayerControllerToRestart->GetPlayerState<AOWPlayerState>())
		{
			EHeroName HeroName = OWPlayerState->GetHeroName();
			if (OWPlayerState->GetHeroName() != EHeroName::None)
			{
				UE_LOG(LogTemp, Log, TEXT("Hero Name in AOWGameModeBase::RestartHero()"));

				FOWHeroInfo HeroInfoToSpawn = HeroInfo->HeroInformation[HeroName];
				ActivateHeroFromPool(PlayerControllerToRestart, HeroName);
			}
		}
	}
}

AOWPlayerStart* AOWGameModeBase::RestartHeroAtPlayerStart(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}

	AActor* StartSpot = ChoosePlayerStart(PlayerController);
	if (!StartSpot)
	{
		UE_LOG(LogTemp, Error, TEXT("There is No Valid Player Start in AOWGameModeBase::ActivateHeroFromPool()"));
		return nullptr;
	}

	AOWPlayerStart* OWPlayerStart = Cast<AOWPlayerStart>(StartSpot);
	if (!OWPlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Cast to OWPlayerStart in AOWGameModeBase::ActivateHeroFromPool()"));
		return nullptr;
	}

	if (!OWPlayerStart->TryClaim(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Already Claimed!"), *OWPlayerStart->GetName());
	}

	return OWPlayerStart; 
}

void AOWGameModeBase::ActivateHeroFromPool(APlayerController* PlayerController, EHeroName HeroName)
{
	UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::ActivateHeroFromPool()"));

	// Check if Player Controller is Valid 
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Player Controller is Not Valid in AOWGameModeBase::ActivateHeroFromPool()"));
		return;
	}

	// Check if Hero is Registerd in Pool and if There is at least One Hero
	if (HeroPool.Contains(HeroName) && HeroPool[HeroName].Heroes.Num() > 0)
	{
		// Pop Hero from Hero Pool 
		FHeroPoolUnit HeroPoolUnit = HeroPool[HeroName].Heroes.Pop();
		AOWCharacter* HeroToUse = Cast<AOWCharacter>(HeroPoolUnit.HeroPawn);

		// Start Hero at Player Start 
		AOWPlayerStart* OWPlayerStart = RestartHeroAtPlayerStart(PlayerController);
		FTransform SpawnTransform = OWPlayerStart->GetActorTransform(); 
	
		HeroToUse->SetActorTransform(SpawnTransform);
		HeroToUse->SetActorHiddenInGame(false);
		HeroToUse->SetActorEnableCollision(true); 
		HeroToUse->SetActorTickEnabled(true);
		HeroToUse->SetOriginalTransform(HeroPoolUnit.HeroTransform);

		PlayerController->Possess(HeroToUse);

		FinishRestartPlayer(PlayerController, SpawnTransform.GetRotation().Rotator()); 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No available Hero in Pool"));
	}
}

void AOWGameModeBase::ChangeHero(APlayerController* PlayerController, EHeroName NewHeroName)
{
	if (!PlayerController) return; 

	AOWCharacter* CurrentHero = Cast<AOWCharacter>(PlayerController->GetPawn()); 

	if (CurrentHero)
	{
		PushHeroToHeroPool(CurrentHero);
	}

	if (AOWPlayerState* OWPlayerState = PlayerController->GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->SetHeroName(NewHeroName);
	}
}

void AOWGameModeBase::PushHeroesToHeroPool()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Cast<APlayerController>(*Iterator); 
		if (!IsValid(PlayerController))
		{
			return;
		}

		AOWCharacter* OldHero = Cast<AOWCharacter>(PlayerController->GetPawn());
		if (!IsValid(OldHero))
		{
			return;
		}
		
		PlayerController->UnPossess();

		PushHeroToHeroPool(OldHero);

		PendingPlayers.Add(PlayerController);
	}
}

void AOWGameModeBase::PushHeroToHeroPool(AOWCharacter* HeroToPush)
{
	if (!IsValid(HeroToPush))
	{
		return;
	}

	FHeroPoolUnit HeroPoolUnit; 
	HeroPoolUnit.HeroPawn = HeroToPush;
	HeroPoolUnit.HeroTransform = HeroToPush->GetOriginalTransform();

	HeroPool[HeroToPush->GetHeroName()].Heroes.Add(HeroPoolUnit);

	DeactivateHero(HeroToPush);
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
	UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::FinishRestartPlayer()"));

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