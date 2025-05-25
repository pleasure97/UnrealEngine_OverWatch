// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/AutoRespawn.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "OWGameplayTags.h"
#include "Message/OWMessageTypes.h"
#include "Interface/CombatInterface.h"


void UAutoRespawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	if (!IsListeningForReset)
	{
		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);

		FGameplayTag RespawnTag = FOWGameplayTags::Get().Gameplay_Message_Respawn; 
		
		FGameplayMessageListenerHandle ListenerHandle =
			GameplayMessageSubsystem.RegisterListener<FHeroRespawnInfo>(RespawnTag, this, &UAutoRespawn::OnHeroRespawned);

		IsListeningForReset = true; 
	}

	if (IsAvatarDeadOrDying())
	{
		//GetAvatarActorFromActorInfo()->
	}
}

void UAutoRespawn::OnHeroRespawned(FGameplayTag Channel, const FHeroRespawnInfo& Payload)
{
	// if (Payload.OwnerPlayerState == GetOwningActorFromActorInfo())
}

bool UAutoRespawn::IsAvatarDeadOrDying()
{
	HasDeadOrDyingAvatar = false; 
	AActor* AvatarActor = GetAvatarActorFromActorInfo(); 
	if (IsValid(AvatarActor))
	{
		if (AvatarActor->Implements<UCombatInterface>())
		{
			HasDeadOrDyingAvatar = ICombatInterface::Execute_IsDead(AvatarActor);
		}
	}
	return HasDeadOrDyingAvatar; 
}

