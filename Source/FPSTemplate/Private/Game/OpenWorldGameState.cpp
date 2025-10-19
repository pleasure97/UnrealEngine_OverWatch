// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OpenWorldGameState.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"

AOpenWorldGameState::AOpenWorldGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	OpenWorldAbilitySytsemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("OpenWorldAbilitySystemComponent");
	OpenWorldAbilitySytsemComponent->SetIsReplicated(true);
	OpenWorldAbilitySytsemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AOpenWorldGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AOpenWorldGameState::MulticastReliableMessageToClients_Implementation(const FOWVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}
