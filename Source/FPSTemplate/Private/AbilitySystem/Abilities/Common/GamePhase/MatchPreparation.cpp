// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/GamePhase/MatchPreparation.h"
#include "AbilitySystem/OWGlobalAbilitySystem.h"
#include "Kismet/GameplayStatics.h"
#include "Game/OWGameState.h"
#include "Message/OWMessageTypes.h"
#include "OWGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "AbilitySystemComponent.h"
#include "Game/OWGamePhaseSubsystem.h"

void UMatchPreparation::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	UOWGlobalAbilitySystem* GlobalAbilitySystem = GetWorld()->GetSubsystem<UOWGlobalAbilitySystem>();
	if (GlobalAbilitySystem)
	{
		GlobalAbilitySystem->ApplyEffectToAll(DamageImmuneGamelayEffectClass);

		// Delay until the countdown heartbeat starts
		GetWorld()->GetTimerManager().SetTimer(BeforeCountdownTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				BroadcastCountdownMessage(); 
				
				GetWorld()->GetTimerManager().ClearTimer(BeforeCountdownTimerHandle);
			}), MatchPreparationTime - CountdownDuration, false);
	}
}

void UMatchPreparation::BroadcastCountdownMessage()
{
	// Get Game State 
	AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(this));
	if (!OWGameState)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find Game State in Match Preparation"));
		return;
	}

	// Initialize Remaining Countdown 
	RemainingCountdown = CountdownDuration; 

	// Set Countdown Timer
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// Get GameplayTag Singleton Container
			const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

			// Initialize Verb Message 
			FOWVerbMessage OWVerbMessage;
			OWVerbMessage.Verb = GameplayTags.Gameplay_Message_MatchBeginCountdown;
			OWVerbMessage.Magnitude = RemainingCountdown;
		
			OWGameState->MulticastReliableMessageToClients(OWVerbMessage);

			// Check Dedicated Server using Kismet System Library 
			bool bClient = !UKismetSystemLibrary::IsDedicatedServer(this);
			if (bClient)
			{
				// Get Gameplay Message Subsystem
				UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
				GameplayMessageSubsystem.BroadcastMessage(GameplayTags.Gameplay_Message_MatchBeginCountdown, OWVerbMessage);
			}

			// When Countdown is Over
			if (RemainingCountdown < 0.f)
			{
				GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle); 

				// Check Net Authority 
				if (CurrentActorInfo->IsNetAuthority())
				{
					// Get Global Ability System and Remove Damage Immune Gameplay Effect from All 
					if (UOWGlobalAbilitySystem* GlobalAbilitySystem = GetWorld()->GetSubsystem<UOWGlobalAbilitySystem>())
					{
						GlobalAbilitySystem->RemoveEffectFromAll(DamageImmuneGamelayEffectClass);
					}

					// Get Game Phase Subsystem and Start "Match In Progress" Phase
					if (UWorld* World = CurrentActorInfo->AbilitySystemComponent->GetWorld())
					{
						if (UOWGamePhaseSubsystem* GamePhaseSubsystem = UWorld::GetSubsystem<UOWGamePhaseSubsystem>(World))
						{
							FOWGamePhaseDynamicDelegate MatchInProgressDelegate;
							GamePhaseSubsystem->StartPhase(MatchInProgress, MatchInProgressDelegate);
						}
					}
				}
			}
		}), 1.f, true);
}
