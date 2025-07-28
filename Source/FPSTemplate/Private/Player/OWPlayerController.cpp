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
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(PlayerState))
	{
		OWPlayerState->HeroName = ChosenHero; 
	}
}

void AOWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 

	bPlayerAlive = true; 
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

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(OWContext, 1);
		}
	}
	
	if (UOWInputComponent* OWInputComponent = CastChecked<UOWInputComponent>(InputComponent))
	{
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
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputPressed)) return; 

	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag); 
}

void AOWPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputReleased)) return;

	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void AOWPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputHeld)) return;

	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

void AOWPlayerController::BroadcastOnPlayerStateChanged()
{
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam; 
	if (LastSeenPlayerState != nullptr)
	{
		if (ITeamInterface* PlayerStateWithTeamInterface = Cast<ITeamInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateWithTeamInterface->GetGenericTeamId(); 
			PlayerStateWithTeamInterface->GetTeamChangedDelegate().RemoveAll(this); 
		}
	}

	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam; 
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

