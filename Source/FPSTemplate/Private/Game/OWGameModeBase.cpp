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

			FirstHeroSelectionStartedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenFirstHeroSelectionStarts);
			GamePhaseSubsystem->WhenPhaseStartsOrIsActive(GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, EPhaseTagMatchType::ExactMatch, FirstHeroSelectionStartedDelegate);
			
			FirstHeroSelectionEndedDelegate = FOWGamePhaseTagDelegate::CreateUObject(this, &AOWGameModeBase::HandleWhenFirstHeroSelectionEnds);
			GamePhaseSubsystem->WhenPhaseEnds(GameplayTags.GamePhase_HeroSelection_FirstHeroSelection, EPhaseTagMatchType::ExactMatch, FirstHeroSelectionEndedDelegate);

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
	}
}

void AOWGameModeBase::DeactivateHero(AOWCharacter* Hero)
{
	// Hide Hero and Disable Collision and Actor Tick for Optimization
	Hero->SetActorHiddenInGame(true);
	Hero->SetActorEnableCollision(false);
	Hero->SetActorTickEnabled(false);
}

void AOWGameModeBase::HandleWhenFirstHeroSelectionStarts(const FGameplayTag& PhaseTag, const float PhaseDuration)
{
	// Get World and Check if it's Valid
	UWorld* World = GetWorld(); 
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is Not Valid in AOWGameModeBase::HandleWhenFirstHeroSelectionStarts()"));
		return;
	}

	// Get Game State and Check if it's Valid
	AOWGameState* OWGameState = GetGameState<AOWGameState>();
	if (!IsValid(OWGameState))
	{
		UE_LOG(LogTemp, Error, TEXT("Game State is Not Valid in AOWGameModeBase::HandleWhenFirstHeroSelectionStarts()"));
		return;
	}
	
	// Get Hero Information Data Asset and Check if it's Valid
	UHeroInfo* HeroInfo = OWGameState->HeroInfo; 
	if (!IsValid(HeroInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("Hero Info is Not Valid in AOWGameModeBase::HandleWhenFirstHeroSelectionStarts()"));
		return;
	}

	// Get All Hero Classes
	const TArray<EHeroName>& AllHeroNamees = HeroInfo->GetAllHeroNames(); 

	// Iterate All Hero Classes
	for (EHeroName HeroName : AllHeroNamees)
	{
		// Except 'None' Hero Name
		if (HeroName == EHeroName::None)
		{
			continue;
		}

		// For 2 Teams
		for (int i = 0; i < 2; ++i)
		{
			FHeroSpawnRequest HeroSpawnRequest; 
			HeroSpawnRequest.HeroCharacter = HeroInfo->HeroInformation[HeroName].HeroPawnClass;

			HeroSpawnRequests.Add(HeroSpawnRequest);
		}
	}

	if (HeroSpawnRequests.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AOWGameModeBase::ProcessSpawnQueue, 0.1f, true);
	}
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

void AOWGameModeBase::ProcessSpawnQueue()
{
	// Get World and Check if it's Valid
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is Not Valid in AOWGameModeBase::ProcessSpawnQueue()"));
		return;
	}

	for (int32 i = 0; i < 2; ++i)
	{
		// No More Heroes to Spawn
		if (HeroSpawnRequests.Num() == 0)
		{
			GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
			return;
		}

		// Get Hero Spawn Request from Array
		FHeroSpawnRequest HeroSpawnRequest = HeroSpawnRequests[0]; 
		HeroSpawnRequests.RemoveAt(0);

		// Spawn Hero Deferred
		AOWCharacter* HeroToBeSpawned = World->SpawnActorDeferred<AOWCharacter>(
			HeroSpawnRequest.HeroCharacter,
			HeroPoolTransform, /*Transform*/
			nullptr, /*Owner*/
			nullptr, /*Instigator*/
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

		// Check if Hero to be Spawned is Valid
		if (!IsValid(HeroToBeSpawned))
		{
			UE_LOG(LogTemp, Error, TEXT("Hero to be Spawned is Not Valid in AOWGameModeBase::HandleWhenFirstHeroSelectionStarts()"));
			continue;
		}

		HeroToBeSpawned->SetOriginalTransform(HeroPoolTransform);
		HeroToBeSpawned->FinishSpawning(HeroPoolTransform);

		PushHeroToHeroPool(HeroToBeSpawned);
	}
}

void AOWGameModeBase::EnableHeroSpawning()
{
	if (UWorld* World = GetWorld())
	{
		bCanSpawnHero = true;

		UE_LOG(LogTemp, Log, TEXT("CALL AOWGameModeBase::EnableHeroSpawning()"));

		for (APlayerController* PendingPlayerController : PendingPlayers)
		{
			RestartHero(PendingPlayerController);
		}
		PendingPlayers.Empty();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Get World from Game Mode Base.")); 
	}
}

void AOWGameModeBase::RestartHero(APlayerController* PlayerControllerToRestart)
{
	if (!IsValid(PlayerControllerToRestart))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is Not Valid in AOWGameModeBase::RestartHero()"));
		return;
	}

	if (AOWPlayerState* OWPlayerState = PlayerControllerToRestart->GetPlayerState<AOWPlayerState>())
	{
		EHeroName HeroName = OWPlayerState->GetHeroName();
		if (OWPlayerState->GetHeroName() != EHeroName::None)
		{
			ActivateHeroFromPool(PlayerControllerToRestart, HeroName);
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
		AOWCharacter* HeroToUse = HeroPool[HeroName].Heroes.Pop();

		// Start Hero at Player Start 
		AOWPlayerStart* OWPlayerStart = RestartHeroAtPlayerStart(PlayerController);

		if (!IsValid(OWPlayerStart))
		{
			UE_LOG(LogTemp, Log, TEXT("OWPlayerStart is Not Valid in AOWGameModeBase::ActivateHeroFromPool()"));
			return;
		}
		FTransform SpawnTransform = OWPlayerStart->GetActorTransform(); 

		HeroToUse->SetActorTransform(SpawnTransform);
		HeroToUse->SetActorEnableCollision(true); 
		HeroToUse->SetActorTickEnabled(true);
		HeroToUse->SetOriginalTransform(HeroPoolTransform);
		// Rendering Optimization - bTickAnimationOnSkeletalMeshInit
		HeroToUse->SetActorHiddenInGame(false);
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
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller is Not Valid in AOWGameModeBase::ChangeHero()"));
		return;
	}

	AOWCharacter* CurrentHero = Cast<AOWCharacter>(PlayerController->GetPawn()); 
	if (IsValid(CurrentHero))
	{
		PushHeroToHeroPool(CurrentHero);
	}

	if (AOWPlayerState* OWPlayerState = PlayerController->GetPlayerState<AOWPlayerState>())
	{
		RestartHero(PlayerController);
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

	if (AController* PC = HeroToPush->GetController())
	{
		PC->UnPossess();
	}

	if (HeroPool.Contains(HeroToPush->GetHeroName()))
	{
		FHeroList& HeroList = HeroPool[HeroToPush->GetHeroName()];
		HeroList.Heroes.Add(HeroToPush);
	}
	else
	{
		FHeroList HeroList;
		HeroList.Heroes.Add(HeroToPush);
		HeroPool.Add(TTuple<EHeroName, FHeroList>(HeroToPush->GetHeroName(), HeroList));
	}
	
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