// Fill out your copyright notice in the Description page of Project Setting
#include "Component/HealthPlateManagerComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "OWGameplayTags.h"
#include "Message/OWMessageTypes.h"
#include "UI/Indicator/IndicatorController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "UI/Indicator/IndicatorLibrary.h"
#include "Component/IndicatorManagerComponent.h"

UHealthPlateManagerComponent::UHealthPlateManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 
}

void UHealthPlateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()); 

	// Check Player Controller is Local Controller 
	if (PlayerController && PlayerController->IsLocalController())
	{
		// Get Gameplay Message Subsystem 
		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 

		/* 1. Listen Whether Health Plate is Added */
		// Get Health Plate Add Tag 
		FGameplayTag HealthPlateAddTag = FOWGameplayTags::Get().Gameplay_Message_HealthPlate_Add;

		// Listen for Gameplay Message - Health Plate Add
		FGameplayMessageListenerHandle HealthPlateAddListener =
			GameplayMessageSubsystem.RegisterListener<FHealthPlateInfo>(HealthPlateAddTag, this, &UHealthPlateManagerComponent::OnHealthPlateAdded);

		/* 2. Listen Whether Health Plate is Removed */
		// Get Health Plate Remove Tag 
		FGameplayTag HealthPlateRemoveTag = FOWGameplayTags::Get().Gameplay_Message_HealthPlate_Remove;

		// Listen for Gameplay Message - Health Plate Remove
		FGameplayMessageListenerHandle HealthPlateRemoveListener =
			GameplayMessageSubsystem.RegisterListener<FHealthPlateInfo>(HealthPlateRemoveTag, this, &UHealthPlateManagerComponent::OnHealthPlateRemoved);

		/* 3. Add Health Plate For Self */
		// Get Health Plate Discover Tag 
		FGameplayTag HealthPlateDiscoverTag = FOWGameplayTags::Get().Gameplay_Message_HealthPlate_Discover; 

		// Initialize Health Plate Manager Info to Broadcast 
		FHealthPlateManagerInfo HealthPlateManagerInfo(this); 

		// Broadcast Health Plate Manager Info 
		GameplayMessageSubsystem.BroadcastMessage(HealthPlateDiscoverTag, HealthPlateManagerInfo); 
	}
	else
	{
		DestroyComponent();
	}
}

void UHealthPlateManagerComponent::OnHealthPlateAdded(FGameplayTag Channel, const FHealthPlateInfo& Payload)
{
	RegisterHealthPlateSource(Payload.Pawn);
}

void UHealthPlateManagerComponent::OnHealthPlateRemoved(FGameplayTag Channel, const FHealthPlateInfo& Payload)
{
	UnregisterHealthPlateSource(Payload.Pawn); 
}

void UHealthPlateManagerComponent::RegisterHealthPlateSource(APawn* IndicatedPawn)
{
	// Create Indicator Controller 
	UIndicatorController* Indicator = NewObject<UIndicatorController>(this); 
	if (!Indicator)
	{
		return;
	}
	//check(IndicatorControllerClass); 

	// Setup Indicator Controller
	Indicator->SetIndicatorClass(IndicatorControllerClass); 
	ACharacter* IndicatedCharacter = Cast<ACharacter>(IndicatedPawn); 
	Indicator->SetSceneComponent(IndicatedCharacter->GetCapsuleComponent());
	Indicator->SetBoundingBoxAnchor(FVector(0.5f, 0.5f, 1.f)); 
	Indicator->SetDataObject(IndicatedPawn); 
	Indicator->SetVAlignment(EVerticalAlignment::VAlign_Bottom); 
	Indicator->SetAutoRemoveWhenIndicatorComponentIsNull(true); 
	Indicator->SetClampToScreen(true); 

	// Add Indicated Pawn and Indicator to Indicator Map 
	IndicatorMap[IndicatedPawn] = Indicator;

	UIndicatorManagerComponent* IndicatorManagerComponent = UIndicatorLibrary::GetIndicatorManagerComponent(Cast<AController>(GetOwner())); 
	if (IndicatorManagerComponent)
	{
		IndicatorManagerComponent->AddIndicator(Indicator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Indicator Manager Component is not Valid or Current Environment is Server.")); 
	}
	
}

void UHealthPlateManagerComponent::UnregisterHealthPlateSource(APawn* IndicatedPawn)
{
	if (IndicatorMap.Find(IndicatedPawn))
	{
		IndicatorMap[IndicatedPawn]->UnregisterIndicator(); 
		IndicatorMap.Remove(IndicatedPawn); 
	}
}

void UHealthPlateManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// TODO - Maybe need to Cancel 'Health Plate for Adding and Removing' Listener
	for (auto& IndicatorPair : IndicatorMap)
	{
		UIndicatorController* Indicator = IndicatorPair.Value;
		Indicator->UnregisterIndicator(); 
	}

	IndicatorMap.Empty();

	Super::EndPlay(EndPlayReason); 
}
