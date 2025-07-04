// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PlayerPerformanceComponent.h"
#include "OWGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/OWGameState.h"
#include "GameFramework/Playerstate.h"
#include "Team/OWTeamSubsystem.h"

void UPlayerPerformanceComponent::BeginPlay()
{
	Super::BeginPlay();

	StartPerformanceListening(); 

	// Call PruneEntries() every second 
	GetWorld()->GetTimerManager().SetTimer(
		PruneTimerHandle,
		this,
		&UPlayerPerformanceComponent::PrunePerformanceHistory,	// Call
		1.f,	// Every second
		true	// Loop
	); 
}

void UPlayerPerformanceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove Any Listener Handles
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 

	// Unregister Listener Handle in Gameplay Message Subsystem
	for (FGameplayMessageListenerHandle& ListenerHandle : ListenerHandles)
	{
		GameplayMessageSubsystem.UnregisterListener(ListenerHandle); 
	}

	// Reset Listener Handles Array 
	ListenerHandles.Empty();
}

void UPlayerPerformanceComponent::StartPerformanceListening()
{
	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	// Register Listener and Add Listener Handle - When Hero is Damaged or Healed
	AddListenerHandle(
		GameplayMessageSubsystem.RegisterListener(
			GameplayTags.Gameplay_Message_HeroDamaged,
			this,
			&UPlayerPerformanceComponent::OnPlayerDamagedMessage));


	// RegisterListener and Add Listener Handle - When Hero is Debuffed 
	AddListenerHandle(
		GameplayMessageSubsystem.RegisterListener(
			GameplayTags.Gameplay_Message_HeroDebuffed,
			this,
			&UPlayerPerformanceComponent::OnPlayerDebuffedMessage)); 

	// Reigster Listener and Add Listener Handle - When Hero is Killed 
	AddListenerHandle(
		GameplayMessageSubsystem.RegisterListener(
			GameplayTags.Gameplay_Message_HeroKilled,
			this,
			&UPlayerPerformanceComponent::OnPlayerKilledMessage));

}

void UPlayerPerformanceComponent::AddListenerHandle(FGameplayMessageListenerHandle&& Handle)
{
	// Add Listener Handle of Movement Constructor
	ListenerHandles.Add(MoveTemp(Handle)); 
}

double UPlayerPerformanceComponent::GetServerTime() const
{
	if (AGameStateBase* GameStateBase = GetWorld()->GetGameState())
	{
		return GameStateBase->GetServerWorldTimeSeconds(); 
	}
	else
	{
		return 0.0; 
	}
}

void UPlayerPerformanceComponent::PrunePerformanceHistory()
{
	// Get Server Time 
	double Now = GetServerTime(); 
	
	// Iterate Performance History 
	for (TPair<TObjectPtr<APlayerState>, FPlayerPerformanceTracking> PerformanceHistoryPair : PerformanceHistory)
	{
		// Get Each Performance Player Tracking 
		FPlayerPerformanceTracking& PlayerPerformanceTracking = PerformanceHistoryPair.Value; 
		// Iterate Each Player's Accumulated Damage History 
		for (auto& AccumulatedDamagePair : PlayerPerformanceTracking.AccumulatedDamageByPlayer)
		{
			// Remove Damaged Info which has Damage Elapsed Seconds is more than Prune Time (5s)
			TArray<FHeroDamagedInfo>& HeroDamagedInfoArray = AccumulatedDamagePair.Value; 
			HeroDamagedInfoArray.RemoveAll([&](const FHeroDamagedInfo& HeroDamagedInfo)
				{
					return (Now - HeroDamagedInfo.DamageTimeSeconds) > PruneTime;
				}); 
		}
		// Iterate Each Player's Accumulated Debuff History 
		for (auto& AccumulatedDebuffPair : PlayerPerformanceTracking.AccumulatedDebuffByPlayer)
		{
			// Remove Debuffed Info which has Damage Elapsed Seconds is more than Prune Time (5s)
			TArray<FHeroDebuffedInfo>& HeroDebuffedInfoArray = AccumulatedDebuffPair.Value;
			HeroDebuffedInfoArray.RemoveAll([&](const FHeroDebuffedInfo& HeroDebuffedInfo)
				{
					return (Now - HeroDebuffedInfo.DebuffTimeSeconds) > PruneTime;
				});
		}
	}
}

void UPlayerPerformanceComponent::OnPlayerDamagedMessage(FGameplayTag Channel, const FHeroDamagedInfo& Payload)
{
	// Prevent Saving Self-Harming Situation
	if (Payload.SourcePlayerState != Payload.TargetPlayerState)
	{
		PerformanceHistory.FindOrAdd(Payload.TargetPlayerState); 
	}
}

void UPlayerPerformanceComponent::OnPlayerDebuffedMessage(FGameplayTag Channel, const FHeroDebuffedInfo& Payload)
{
	if (GetOwner()->HasAuthority())
	{
		if (AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(this)))
		{
			// TODO - Change from FHeroDebuffedInfo to OWVerbMessage when Debuff Message is Broadcasted in Attribute Set 
			// OWGameState->MulticastReliableMessageToClients(Payload);

			if (UKismetSystemLibrary::IsDedicatedServer(this))
			{
				return;
			}
		}
	}
}

void UPlayerPerformanceComponent::OnPlayerKilledMessage(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	if (GetOwner()->HasAuthority())
	{
		if (AOWGameState* OWGameState = Cast<AOWGameState>(UGameplayStatics::GetGameState(this)))
		{
			OWGameState->MulticastReliableMessageToClients(Payload); 

			if (UKismetSystemLibrary::IsDedicatedServer(this))
			{
				return;
			}
		}
	}

	if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		APlayerState* InstigatorPlayerState = Cast<APlayerState>(Payload.Instigator); 
		APlayerState* TargetPlayerState = Cast<APlayerState>(Payload.Target);

		FHeroKilledInfo HeroKilledInfo; 
		HeroKilledInfo.InstigatorPlayerState = InstigatorPlayerState; 
		HeroKilledInfo.TargetPlayerState = TargetPlayerState; 
		HeroKilledInfo.InstigatorTags = Payload.InstigatorTags;

		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 
		GameplayMessageSubsystem.BroadcastMessage(FOWGameplayTags::Get().Gameplay_Message_KillFeed, HeroKilledInfo); 
	}
}
