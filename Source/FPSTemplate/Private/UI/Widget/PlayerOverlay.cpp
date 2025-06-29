// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerOverlay.h"
#include "OWGameplayTags.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/HealthBarPool.h"
#include "UI/Widget/PlayerHealthStatus.h"
#include "UI/Widget/PlayerSkills.h"
#include "UI/Widget/PlayerWeaponStatus.h"
#include "UI/Widget/UltimateGauge.h"
#include "UI/Widget/HitIndicatorPool.h"
#include "UI/Widget/CombatLogPool.h"
#include "UI/Widget/KillLogPool.h"
#include "UI/Widget/Assault/AssaultOverlay.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

	// Register Listener and Add Listener Handle - When Hero is Debuffed
	AddListenerHandle(
		GameplayMessageSubsystem.RegisterListener(
			GameplayTags.Gameplay_Message_HeroDebuffed,
			this,
			&UPlayerOverlay::OnPlayerDebuffedMessage));

	// Register Listener and Add Listener Handle - When Match Begins Countdown 
	AddListenerHandle(
		GameplayMessageSubsystem.RegisterListener(
			GameplayTags.Gameplay_Message_MatchBeginCountdown,
			this,
			&UPlayerOverlay::OnMatchBeginCountdownMessage));
}

void UPlayerOverlay::NativeDestruct()
{
	Super::NativeDestruct(); 

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

void UPlayerOverlay::SetChildWidgetControllers()
{
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		WBP_PlayerHealthBarPool->SetWidgetController(OverlayWidgetController); 
		WBP_PlayerHealthStatus->SetWidgetController(OverlayWidgetController);
		WBP_PlayerSkills->SetWidgetController(OverlayWidgetController);
		WBP_PlayerWeaponStatus->SetWidgetController(OverlayWidgetController);
		WBP_UltimateGauge->SetWidgetController(OverlayWidgetController);
		WBP_HitIndicatorPool->SetWidgetController(OverlayWidgetController);
		WBP_CombatLogPool->SetWidgetController(OverlayWidgetController);
		WBP_KillLogPool->SetWidgetController(OverlayWidgetController);
		WBP_AssaultOverlay->SetWidgetController(OverlayWidgetController);
	}
}

void UPlayerOverlay::AddListenerHandle(FGameplayMessageListenerHandle&& Handle)
{
	ListenerHandles.Add(MoveTemp(Handle)); 
}

void UPlayerOverlay::OnPlayerDebuffedMessage(FGameplayTag Channel, const FHeroDebuffedInfo& Payload)
{

}

void UPlayerOverlay::OnMatchBeginCountdownMessage(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	if (TextBlock_GameplayMessage)
	{
		int32 CountdownTime = FMath::TruncToInt(Payload.Magnitude); 
		FText CountdownText = FText::AsNumber(CountdownTime);
		TextBlock_GameplayMessage->SetText(CountdownText);
	}
}

