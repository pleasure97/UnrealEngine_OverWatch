// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tank/Reinhardt/BarrierField.h"
#include "GameFramework/Character.h"
#include "UI/Widget/Heroes/ReinhardtBarrierWidget.h"
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
		
		// Set Owner of Blink Widget to Local Player Controller
		if (APlayerController* AvatarPlayerController = Cast<APlayerController>(AvatarCharacter->GetController()))
		{
			if (AvatarPlayerController->IsLocalController())
			{
				// Create Blink Widget and Add to Viewport in Desired Size
				BarrierWidget = CreateWidget<UReinhardtBarrierWidget>(AvatarPlayerController, BarrierWidgetClass);
				if (BarrierWidget)
				{
					int32 ViewWidth, ViewHeight;
					AvatarPlayerController->GetViewportSize(ViewWidth, ViewHeight);
					BarrierWidget->AddToPlayerScreen();
					BarrierWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
					BarrierWidget->SetPositionInViewport(FVector2D(ViewWidth * 0.5f, ViewHeight * 0.35f), true);
					BarrierWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}

void UBarrierField::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (IsValid(EnergyBarrier))
		{
			EnergyBarrier->Destroy();
		}
	}
	else
	{
		if (IsValid(BarrierWidget))
		{
			BarrierWidget->RemoveFromParent();
			BarrierWidget = nullptr;
		}
	}

	Super::OnRemoveAbility(ActorInfo, Spec);
}
