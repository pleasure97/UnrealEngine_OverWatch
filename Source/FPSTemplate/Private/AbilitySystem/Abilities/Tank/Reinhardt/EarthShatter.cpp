// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tank/Reinhardt/EarthShatter.h"
#include "Actor/EarthShatterShockwave.h"
#include "Character/OWCharacter.h"
#include "Camera/CameraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "OWGameplayTags.h"

void UEarthShatter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FOWGameplayTags::Get().Event_Reinhardt_EarthShatterFinished, nullptr, true); 
	if (IsValid(WaitGameplayEventTask))
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UEarthShatter::OnEarthShatterFinished);
		WaitGameplayEventTask->ReadyForActivation();
	}

	// TODO - Handle When Avatar Actor is of Class Other than 'OWCharacter'
	AOWCharacter* OWCharacter = Cast<AOWCharacter>(GetAvatarActorFromActorInfo()); 
	// Check if OWCharacter is Valid 
	if (!IsValid(OWCharacter))
	{
		return;
	}

	// TODO - Set Accurate Location 
	// Setup Shockwave Transform 
	FTransform ShockwaveTransform;
	FVector ForwardVector = OWCharacter->GetFirstPersonCamera()->GetForwardVector();
	ShockwaveTransform.SetLocation(OWCharacter->GetActorLocation() + ForwardVector * 100.f);
	ShockwaveTransform.SetRotation(ForwardVector.ToOrientationQuat());

	// Spawn 'EarthShatter Shockwave' Deferred 
	AEarthShatterShockwave* SpawnedShockwave
		= GetWorld()->SpawnActorDeferred<AEarthShatterShockwave>(EarthShatterShockwaveClass, ShockwaveTransform);
	// Check if Earth Shatter Shockwave is Valid 
	if (!IsValid(SpawnedShockwave))
	{
		UE_LOG(LogTemp, Error, TEXT("EarthShatterShockwave is Not Spawned in UEarthShatter::ActivateAbility()"));
		return;
	}

	// Set Owner
	SpawnedShockwave->SetOwner(OWCharacter);

	// Setup Shockwave Settings 
	SpawnedShockwave->InitializeShockwave(
		MaxDistance, ExpansionSpeed, ArcAngle, VerticalHeight,
		ShockwaveDirectHitDistance, ShockwaveDirectHitDamage, ShockwaveBaseDamage,
		MakeDamageEffectParamsFromClassDefaults());

	// Finish Shockwave Spawning 
	SpawnedShockwave->FinishSpawning(ShockwaveTransform);

	// TODO - Gameplay Cue 
}

void UEarthShatter::OnEarthShatterFinished(FGameplayEventData Payload)
{
	if (IsValid(WaitGameplayEventTask))
	{
		WaitGameplayEventTask->EventReceived.RemoveAll(this); 
		WaitGameplayEventTask->EndTask();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true /*bReplicateEndAbility*/, false/*bWasCancelled*/);
}
