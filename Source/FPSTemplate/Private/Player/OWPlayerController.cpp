// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/OWInputComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "Game/OWGameModeBase.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"
#include "UserSettings/EnhancedInputUserSettings.h"

AOWPlayerController::AOWPlayerController()
{
	bReplicates = true; 
	bPlayerAlive = true; 
}

void AOWPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
}

FGenericTeamId AOWPlayerController::GetGenericTeamId() const
{
	if (const ITeamInterface* PlayerStateWithTeamInterface = Cast<ITeamInterface>(PlayerState))
	{
		return PlayerStateWithTeamInterface->GetGenericTeamId(); 
	}

	return FGenericTeamId::NoTeam;
}

FOnTeamIndexChangedDelegate* AOWPlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate; 
}

FOnPlayerStateChanged& AOWPlayerController::GetOnPlayerStateChangedDelegate()
{
	return OnPlayerStateChanged;
}

void AOWPlayerController::ClientHeroDamaged_Implementation(const FHeroDamagedInfo& HeroDamagedInfo)
{
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 
	GameplayMessageSubsystem.BroadcastMessage(FOWGameplayTags::Get().Gameplay_Message_HeroDamaged, HeroDamagedInfo); 
}

void AOWPlayerController::ShowWidget(TSubclassOf<UUserWidget> InUserWidget)
{
	TArray<UUserWidget*> CollapsedWidgets; 
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, CollapsedWidgets, InUserWidget); 
	for (UUserWidget* CollapsedWidget : CollapsedWidgets)
	{
		CollapsedWidget->SetVisibility(ESlateVisibility::Visible); 
	}
}

void AOWPlayerController::CollapseWidget(TSubclassOf<UUserWidget> InUserWidget)
{
	TArray<UUserWidget*> VisibleWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, VisibleWidgets, InUserWidget);
	for (UUserWidget* VisibleWidget : VisibleWidgets)
	{
		VisibleWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AOWPlayerController::ServerChooseHero_Implementation(EHeroName ChosenHero)
{
	// Set Hero Name of Player State
	AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState);
	if (!IsValid(OWPlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("Player State is Not Valid in AOWPlayerController::ServerChooseHero()"));
		return;
	}
	OWPlayerState->HeroName = ChosenHero;

	// Check if Has Authority
	if (!HasAuthority())
	{
		return;
	}

	// Get Game Phase Subsystem
	UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>(); 
	if (!IsValid(GamePhaseSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("GamePhaseSubsystem is Not Valid in AOWPlayerController::ServerChooseHero()"));
		return;
	}

	// Check if GamePhase is 'Match In Progress' or 'Match Preparation'
	FGameplayTag MatchInProgressTag = FGameplayTag::RequestGameplayTag(FName("GamePhase.MatchInProgress")); 
	FGameplayTag MatchPreparationTag = FGameplayTag::RequestGameplayTag(FName("GamePhase.MatchPreparation"));
	if (GamePhaseSubsystem->IsPhaseActive(MatchInProgressTag) || GamePhaseSubsystem->IsPhaseActive(MatchPreparationTag))
	{
		if (AOWGameModeBase* OWGameMode = Cast<AOWGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			OWGameMode->ChangeHero(this, OWPlayerState->HeroName);
		}
	}
}

void AOWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 

	bPlayerAlive = true; 

	UOWGameUserSettings* OWGameUserSettings = UOWGameUserSettings::Get(); 

	if (OWGameUserSettings->GetLastCPUBenchmarkResult() == -1.f || OWGameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		OWGameUserSettings->RunHardwareBenchmark(); 
		OWGameUserSettings->ApplyHardwareBenchmarkResults();
	}
}

void AOWPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AOWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); 

	if (IsLocalController())
	{
		check(OWContext);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings(); 

			UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings(); 
			if (UserSettings)
			{
				UserSettings->RegisterInputMappingContext(OWContext);
				UserSettings->ApplySettings();
			}

			Subsystem->AddMappingContext(OWContext, 1);
		}
	}
	
	if (UOWInputComponent* OWInputComponent = CastChecked<UOWInputComponent>(InputComponent))
	{
		OWInputComponent->ClearActionEventBindings();
		OWInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Input_Move);
		OWInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Input_Look);
		OWInputComponent->BindAbilityActions(InputConfig, this,
			&AOWPlayerController::AbilityInputTagPressed, &AOWPlayerController::AbilityInputTagReleased, &AOWPlayerController::AbilityInputTagHeld);
	}
}

void AOWPlayerController::InitPlayerState()
{
	Super::InitPlayerState(); 
	BroadcastOnPlayerStateChanged(); 
}

void AOWPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState(); 
	BroadcastOnPlayerStateChanged();
}

void AOWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	BroadcastOnPlayerStateChanged();

	OnPlayerStateChanged.Broadcast(PlayerState);
}

UOWAbilitySystemComponent* AOWPlayerController::GetAbilitySystemComponent()
{
	if (!OWAbilitySystemComponent)
	{
		OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return OWAbilitySystemComponent;
}

void AOWPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!bPlayerAlive) return;

	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputPressed)) return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AOWPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); 
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}

void AOWPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// Get GameplayTag Singleton Container
	const FOWGameplayTags& OWGameplayTags = FOWGameplayTags::Get(); 
	// Get Custom Ability System Component
	UOWAbilitySystemComponent* OWASC = GetAbilitySystemComponent(); 

	// Check if Input Tag Matches Escape Key 
	bool bIsEscapeKey = InputTag.MatchesTagExact(OWGameplayTags.InputTag_Escape);

	// When 'Player_Block_InputPressed' Gameplay Tag is Present, Block when a Key Other than Escape Key is Pressed
	if (!bIsEscapeKey)
	{
		if (IsValid(OWASC) && OWASC->HasMatchingGameplayTag(OWGameplayTags.Player_Block_InputPressed))
		{
			return;
		}
	}
	
	if (IsValid(OWASC))
	{
		OWASC->AbilityInputTagPressed(InputTag);
	}
}

void AOWPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// Get GameplayTag Singleton Container
	const FOWGameplayTags& OWGameplayTags = FOWGameplayTags::Get();
	// Get Custom Ability System Component
	UOWAbilitySystemComponent* OWASC = GetAbilitySystemComponent();

	// Check if Input Tag Matches Escape Key 
	bool bIsEscapeKey = InputTag.MatchesTagExact(OWGameplayTags.InputTag_Escape);

	// When 'Player_Block_InputReleased' Gameplay Tag is Present, Block when a Key Other than Escape Key is Pressed
	if (!bIsEscapeKey)
	{
		if (IsValid(OWASC) && OWASC->HasMatchingGameplayTag(OWGameplayTags.Player_Block_InputReleased))
		{
			return;
		}
	}

	if (IsValid(OWASC))
	{
		OWASC->AbilityInputTagReleased(InputTag);
	}
}

void AOWPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// Get GameplayTag Singleton Container
	const FOWGameplayTags& OWGameplayTags = FOWGameplayTags::Get();
	// Get Custom Ability System Component
	UOWAbilitySystemComponent* OWASC = GetAbilitySystemComponent();

	// Check if Input Tag Matches Escape 
	bool bIsEscapeKey = InputTag.MatchesTagExact(OWGameplayTags.InputTag_Escape);

	// When 'Player_Block_InputReleased' Gameplay Tag is Present, Block when a Key Other than Escape Key is Pressed
	if (!bIsEscapeKey)
	{
		if (IsValid(OWASC) && OWASC->HasMatchingGameplayTag(OWGameplayTags.Player_Block_InputHeld))
		{
			return;
		}
	}

	if (IsValid(OWASC))
	{
		OWASC->AbilityInputTagHeld(InputTag);
	}
}

void AOWPlayerController::BroadcastOnPlayerStateChanged()
{
	int32 OldTeamID = -1; 
	if (LastSeenPlayerState != nullptr)
	{
		if (ITeamInterface* PlayerStateWithTeamInterface = Cast<ITeamInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateWithTeamInterface->GetGenericTeamId(); 
			PlayerStateWithTeamInterface->GetTeamChangedDelegate().RemoveAll(this); 
		}
	}

	int32 NewTeamID = -1;
	if (PlayerState != nullptr)
	{
		if (ITeamInterface* PlayerStateWithTeamInterface = Cast<ITeamInterface>(PlayerState))
		{
			NewTeamID = PlayerStateWithTeamInterface->GetGenericTeamId(); 
			PlayerStateWithTeamInterface->GetTeamChangedDelegate().AddDynamic(this, &AOWPlayerController::OnPlayerStateChangedTeam); 
		}
	}

	BroadcastTeamChanged(this, OldTeamID, NewTeamID); 

	LastSeenPlayerState = PlayerState; 
}

void AOWPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	BroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam)); 
}

