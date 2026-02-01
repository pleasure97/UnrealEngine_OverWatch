// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tank/Reinhardt/BarrierField.h"
#include "GameFramework/Character.h"
#include "Actor/EnergyBarrier.h"
#include "Components/BoxComponent.h"

void UBarrierField::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// Check if Actor Info and Avatar Actor is Valid
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return;
	}

	// Check if Avatar Actor is Locally Controlled
	if (ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		// Check if World is Valid 
		if (ActorInfo->IsNetAuthority())
		{
			UWorld* World = GetWorld();
			if (!IsValid(World))
			{
				UE_LOG(LogTemp, Error, TEXT("World is Not Valid in UBarrierField::OnGiveAbility()"));
				return;
			}

			// Spawn Energy Barrier in World 
			EnergyBarrier = World->SpawnActor<AEnergyBarrier>(EnergyBarrierClass, AvatarCharacter->GetTransform());
			if (!IsValid(EnergyBarrier))
			{
				UE_LOG(LogTemp, Error, TEXT("Casting Energy Barrier is Not Valid in UBarrierField::OnGiveAbility()"));
				return;
			}
			// Disable Collision of Energy Barrier and Hide Energy Barrier
			EnergyBarrier->AttachToComponent(AvatarCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BarrierSocket"));
			EnergyBarrier->Box->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			EnergyBarrier->SetActorHiddenInGame(true);
		}
	}
}
