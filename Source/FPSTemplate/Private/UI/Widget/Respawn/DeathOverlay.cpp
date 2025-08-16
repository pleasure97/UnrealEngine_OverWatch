// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/Respawn/DeathOverlay.h"
#include "UI/Widget/Respawn/RespawnGauge.h"
#include "Components/Button.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Team/OWTeamSubsystem.h"
#include "OWGameplayTags.h"
#include "Interface/CombatInterface.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Widget/HeroSelection/HeroSelectionOverlay.h"

void UDeathOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Bind Button Clicked 
	if (Button_ChangeSpectator)
	{
		Button_ChangeSpectator->OnClicked.AddDynamic(this, &UDeathOverlay::ChangeSpectator); 
		ChangeSpectator(); 
	}

	if (RespawnTime > 0.f)
	{
		if (WBP_RespawnGauge)
		{
			WBP_RespawnGauge->InitializeRespawnGauge(RespawnTime);
		}
	}
}

void UDeathOverlay::NativeDestruct()
{
	// Remove Binding of Button Clicked
	if (Button_ChangeSpectator)
	{
		Button_ChangeSpectator->OnClicked.RemoveAll(this); 
	}

	Super::NativeDestruct();
}

void UDeathOverlay::ChangeSpectator()
{
	TArray<APawn*> LiveTeamMembers; 
	GetAllLiveTeamMembers(LiveTeamMembers);

	// If All Team Members are dead, Show Hero Selection Overlay to Cover Up the State of Nothing 
	if (LiveTeamMembers.Num() == 0)
	{
		TArray<UUserWidget*> HeroSelectionWidgets; 
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), HeroSelectionWidgets, HeroSelectionOverlayWidgetClass); 
		
		for (UUserWidget* HeroSelectionWidget : HeroSelectionWidgets)
		{
			HeroSelectionWidget->SetVisibility(ESlateVisibility::Visible); 
		}
		return; 
	}

	// Rotate Current Watch Index to See Another Live Team Members 
	CurrentWatchIndex = (CurrentWatchIndex + 1) % LiveTeamMembers.Num(); 

	// Set View Target to Next Player Controller 
	APawn* NextPawn = LiveTeamMembers[CurrentWatchIndex]; 
	if (NextPawn)
	{
		GetOwningPlayer()->SetViewTargetWithBlend(NextPawn);
	}
}

void UDeathOverlay::GetAllLiveTeamMembers(TArray<APawn*>& OutPawns) const
{
	OutPawns.Reset();

	if (UWorld* World = GetWorld())
	{
		// Get Game State 
		if (AGameStateBase* GameState = UGameplayStatics::GetGameState(World))
		{
			// Get Player Array from Game State
			for (APlayerState* PlayerState : GameState->PlayerArray)
			{
				// Get Team Subsystem 
				if (UOWTeamSubsystem* TeamSubsystem = World->GetSubsystem<UOWTeamSubsystem>())
				{
					// Check if Another Player State is on Same Team 
					if (TeamSubsystem->CompareTeams(PlayerState, GetOwningPlayerState()) == EOWTeamComparison::OnSameTeam)
					{
						// Check Player State is not my own 
						if (PlayerState != GetOwningPlayerState())
						{
							// Check Owning Pawn is not Dead
							if (!ICombatInterface::Execute_IsDead(PlayerState->GetPawn()))
							{
								OutPawns.Add(PlayerState->GetPawn()); 
							}
						}
					}
				}
			}
		}
	}
}