// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/OWGamePhaseAbility.h"
#include "AbilitySystemComponent.h"
#include "Game/OWGamePhaseSubsystem.h"

UOWGamePhaseAbility::UOWGamePhaseAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo; 
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; 
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated; 
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly; 
}

void UOWGamePhaseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo->IsNetAuthority())
	{
		UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld(); 
		UOWGamePhaseSubsystem* GamePhaseSubsystem = UWorld::GetSubsystem<UOWGamePhaseSubsystem>(World); 
		GamePhaseSubsystem->OnBeginPhase(this, Handle); 
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 
}

void UOWGamePhaseAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActorInfo->IsNetAuthority())
	{
		UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld(); 
		UOWGamePhaseSubsystem* GamePhaseSubsystem = UWorld::GetSubsystem<UOWGamePhaseSubsystem>(World); 
		GamePhaseSubsystem->OnEndPhase(this, Handle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, false); 
}

