// Fill out your copyright notice in the Description page of Project Settings.
#include "Component/HealthPlateSourceComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/AsyncAction_ListenForGameplayMessage.h"
#include "OWGameplayTags.h"
#include "Message/OWMessageTypes.h"
#include "Component/HealthPlateManagerComponent.h"

UHealthPlateSourceComponent::UHealthPlateSourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthPlateSourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get Gameplay Message Subsystem
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 

	/* 1. Add Health Plate For Self */
	// Set Health Plate Add Tag To Broadcast
	FGameplayTag HealthPlateAddTag = FOWGameplayTags::Get().Gameplay_Message_HealthPlate_Add; 

	// Construct Health Plate Info to Broadcast
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); 
	FHealthPlateInfo HealthPlateInfo = FHealthPlateInfo(OwnerPawn); 

	// Broadcast Health Plate Info 
	GameplayMessageSubsystem.BroadcastMessage(HealthPlateAddTag, HealthPlateInfo);

	/* 2. Listen for HealthPlateManagerComponent */

	// Set Health Plate Discover Tag to Broadcast
	FGameplayTag HealthPlateDiscoverTag = FOWGameplayTags::Get().Gameplay_Message_HealthPlate_Discover;

	FGameplayMessageListenerHandle ListenerHandle = 
		GameplayMessageSubsystem.RegisterListener<FHealthPlateManagerInfo>(HealthPlateDiscoverTag, this, &UHealthPlateSourceComponent::OnHealthPlateDiscovered);
}

void UHealthPlateSourceComponent::OnHealthPlateDiscovered(FGameplayTag Channel, const FHealthPlateManagerInfo& Payload)
{
	UHealthPlateManagerComponent* HealthPlateManager = Payload.HealthPlateManagerComponent;
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); 
	HealthPlateManager->RegisterHealthPlateSource(OwnerPawn); 
}


void UHealthPlateSourceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Get Gameplay Message Subsystem
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);

	// Set Health Plate Remove Tag To Broadcast
	FGameplayTag HealthPlateRemoveTag = FOWGameplayTags::Get().Gameplay_Message_HealthPlate_Remove;

	// Construct Health Plate Info to Broadcast
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	FHealthPlateInfo HealthPlateInfo = FHealthPlateInfo(OwnerPawn);

	// Broadcast Health Plate Info 
	GameplayMessageSubsystem.BroadcastMessage(HealthPlateRemoveTag, HealthPlateInfo);

	Super::EndPlay(EndPlayReason); 
}
