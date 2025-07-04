// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DamageMarker.h"
#include "Components/Image.h"
#include "OWGameplayTags.h"
#include "GameFramework/PlayerState.h"

void UDamageMarker::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Set Damage Marker and Kill Icon Visibilities
	if (Image_DamageMarker)
	{
		Image_DamageMarker->SetVisibility(ESlateVisibility::Collapsed); 
	}

	if (Image_KillIcon)
	{
		Image_KillIcon->SetVisibility(ESlateVisibility::Collapsed); 
	}

	// Get Gameplay Message Subsystem 
	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this); 
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	DamageListenerHandle = 
		GameplayMessageSubsystem.RegisterListener<FHeroDamagedInfo>(GameplayTags.Gameplay_Message_HeroDamaged, this, &UDamageMarker::OnEnemyDamaged);

	KillListenerHandle =
		GameplayMessageSubsystem.RegisterListener<FOWVerbMessage>(GameplayTags.Gameplay_Message_HeroKilled, this, &UDamageMarker::OnEnemyKilled);
}

void UDamageMarker::NativeDestruct()
{
	if (DamageListenerHandle.IsValid())
	{
		DamageListenerHandle.Unregister();
	}
	if (KillListenerHandle.IsValid())
	{
		KillListenerHandle.Unregister(); 
	}

	Super::NativeDestruct(); 
}

void UDamageMarker::OnEnemyDamaged(FGameplayTag Channel, const FHeroDamagedInfo& Payload)
{
	// Check if Instigator is Owner and Damage is greater than 0
	if ((Payload.SourcePlayerState == GetOwningPlayerState()) && (Payload.Damage > 0.f))
	{
		// Reset Damage Animation if Damage Animation was already playing 
		if (DamageAnimation && IsAnimationPlayingForward(DamageAnimation))
		{
			StopAnimation(DamageAnimation); 
			SetAnimationCurrentTime(DamageAnimation, 0.f); 
		}
		PlayAnimationForward(DamageAnimation); 
	}
}

void UDamageMarker::OnEnemyKilled(FGameplayTag Channel, const FOWVerbMessage& Payload)
{
	// Check if Instigator is Owner
	if (APlayerState* InstiagtorPlayerState = Cast<APlayerState>(Payload.Instigator))
	{
		if (InstiagtorPlayerState == GetOwningPlayerState())
		{
			// Reset Kill Animation if Kill Animation was already playing
			if (KillAnimation && IsAnimationPlayingForward(KillAnimation))
			{
				StopAnimation(KillAnimation);
				SetAnimationCurrentTime(KillAnimation, 0.f);
			}
			PlayAnimationForward(KillAnimation);
		}
	}
}


