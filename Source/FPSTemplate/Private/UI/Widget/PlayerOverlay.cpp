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
#include "AbilitySystem/OWAbilitySystemLibrary.h"

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
	// Check if Gameplay Message Text Block is Valid
	if (!IsValid(TextBlock_GameplayMessage))
	{
		UE_LOG(LogTemp, Error, TEXT("TextBlock_GameplayMessage is Not Valid in UPlayerOverlay::OnPlayerDebuffedMessage()"));
		return;
	}

	UWorld* World = GetWorld(); 
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is Not Valid in UPlayerOverlay::OnPlayerDebuffedMessage()"));
		return;
	}

	// Check if Message's Source Player State is Owning Player State
	if (Payload.DebuffDuration <= 0.f)
	{
		return;
	}

	// Check if Owning Player State is Victim or Instigator
	bool bVictim;
	if (Payload.TargetPlayerState == GetOwningPlayerState())
	{
		bVictim = true;
	}
	else if (Payload.SourcePlayerState == GetOwningPlayerState())
	{
		bVictim = false;
	}
	else
	{
		return;
	}

	// Get Hero Debuff UI Info Matching Debuff GameplayTag
	UHeroDebuffInfo* HeroDebuffInfo = UOWAbilitySystemLibrary::GetDebuffInfo(World);
	if (!IsValid(HeroDebuffInfo))
	{
		return;
	}
	FHeroDebuffUIInfo* HeroDebuffUIInfo = HeroDebuffInfo->GetHeroDebuffUIInfo(Payload.DebuffTag);
	if (!HeroDebuffUIInfo)
	{
		return;
	}

	// Set Debuff Gameplay Message UI Design
	FText DebuffText = bVictim ? HeroDebuffUIInfo->DebuffVictimUIText : HeroDebuffUIInfo->DebuffInstigatorUIText;
	FLinearColor DebuffTextColor = bVictim ? HeroDebuffUIInfo->DebuffVictimUIColor : HeroDebuffUIInfo->DebuffInstigatorUIColor;
	if (DebuffText.IsEmpty())
	{
		return;
	}

	TextBlock_GameplayMessage->SetText(DebuffText);
	TextBlock_GameplayMessage->SetColorAndOpacity(DebuffTextColor);

	// Set Timer that Lasts as long as Debuff Duration
	World->GetTimerManager().ClearTimer(DebuffMessageTimerHandle);
	World->GetTimerManager().SetTimer(
		DebuffMessageTimerHandle,
		[this]()
		{
			if (TextBlock_GameplayMessage)
			{
				TextBlock_GameplayMessage->SetText(FText::GetEmpty());
			}
		},
		Payload.DebuffDuration,
		false);
}

void UPlayerOverlay::OnMatchBeginCountdownMessage(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	if (TextBlock_GameplayMessage)
	{
		int32 CountdownTime = FMath::TruncToInt(Payload.Magnitude);
		FText CountdownText = FText::AsNumber(CountdownTime);
		TextBlock_GameplayMessage->SetText(CountdownText);
		TextBlock_GameplayMessage->SetColorAndOpacity(FColor::White);
		if (CountdownTime == 0.f)
		{
			FText EmptyText = FText::FromString("");
			TextBlock_GameplayMessage->SetText(EmptyText);
		}
	}
}

