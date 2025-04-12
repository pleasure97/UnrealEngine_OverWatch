// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/DemolishPylon.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Actor/HealingSunStone.h"
#include "UI/Widget/OWUserWidget.h"

bool UDemolishPylon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags); 
}

void UDemolishPylon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	// Set Owner Character
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()); 
	// Set Player Controller
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()); 

	// Project World Location to Widget Position 
	FVector2D ScreenPosition; 
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, Pylon->GetActorLocation(), ScreenPosition, false);

	// Demolish Pylon 
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (bIsServer)
	{
		Pylon->Destroy();
	}

	// Create Pylon Destroyed Widget
	if (IsLocallyControlled())
	{
		PylonDestroyedWidget = CreateWidget<UOWUserWidget>(PlayerController, PylonDestroyedWidgetClass); 
		PylonDestroyedWidget->AddToViewport(); 
		PylonDestroyedWidget->SetPositionInViewport(ScreenPosition, false); 
		GetWorld()->GetTimerManager().SetTimer(RemoveWidgetTimerHandle, this, &UDemolishPylon::RemoveWidget, 2.f, false); 
	}
}

void UDemolishPylon::RemoveWidget()
{
	if (PylonDestroyedWidget)
	{
		PylonDestroyedWidget->RemoveFromParent(); 
	}
}