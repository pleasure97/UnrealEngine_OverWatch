// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/GamePhase/PostMatch.h"
#include "AbilitySystem/OWGlobalAbilitySystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/DSSystemStatics.h"

void UPostMatch::ActivateAbility(
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

		// Delay by Next Game Interval
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				UDSSystemStatics::PlayNextGame(this); 
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			}), NextGameInterval, false);
	}

	// TODO
	// 1. Create post match widget which displays game results
	// 2. If Confirmation Button is clicked, check if post match game phase is active, and play next game 
}