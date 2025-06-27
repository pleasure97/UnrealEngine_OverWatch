// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/GamePhase/HeroSelectionPhase.h"
#include "OWGameplayTags.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayCueFunctionLibrary.h"
#include "GameFramework/GameStateBase.h"

void UHeroSelectionPhase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayCueParameters HeroSelectionCueParameters; 
	HeroSelectionCueParameters.RawMagnitude = HeroSelectionTime; 

	ActivateHeroSelectionCue(FOWGameplayTags::Get().GameplayCue_HeroSelection, HeroSelectionCueParameters);

	// Delay until the hero selection time ends 
	GetWorld()->GetTimerManager().SetTimer(HeroSelectionTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// TODO - What if the client didn't select the hero?

			ClearHeroSelectionCue(); 

			UOWGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UOWGamePhaseSubsystem>();
			if (GamePhaseSubsystem)
			{
				FOWGamePhaseDynamicDelegate MatchPrepartionDelegate;
				GamePhaseSubsystem->StartPhase(MatchPreparation, MatchPrepartionDelegate);
			}

			GetWorld()->GetTimerManager().ClearTimer(HeroSelectionTimerHandle);

		}), HeroSelectionTime, false);
}

void UHeroSelectionPhase::ClearHeroSelectionCue()
{
	// Check if Active Hero Selection Cue is Valid 
	if (ActiveHeroSelectionCue.IsValid())
	{
		// Get Game State
		AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(this);
		// Declare Default Constructor of Gameplay Cue Parameters
		FGameplayCueParameters GameplayCueParameters; 
		// Remove Gameplay Cue On Actor (Looping)
		UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(GameStateBase, ActiveHeroSelectionCue, GameplayCueParameters); 
		// Reset Active Hero Selection Gameplay Cue 
		ActiveHeroSelectionCue = FGameplayTag::EmptyTag; 
	}
}

void UHeroSelectionPhase::ActivateHeroSelectionCue(const FGameplayTag GameplayCueTag, FGameplayCueParameters& GameplayCueParameters)
{
	ClearHeroSelectionCue(); 

	// Initialize Active Hero Selection Cue 
	ActiveHeroSelectionCue = GameplayCueTag; 
	// Get Game State
	AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(this);
	// Add Gameplay Cue On Actor (Looping)
	UGameplayCueFunctionLibrary::AddGameplayCueOnActor(GameStateBase, ActiveHeroSelectionCue, GameplayCueParameters);
}

