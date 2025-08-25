// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Damage/Tracer/TracerBlink.h"
#include "GameFramework/Character.h"
#include "UI/Widget/Heroes/TracerBlinkWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UTracerBlink::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
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
		if (!AvatarCharacter->IsLocallyControlled())
		{
			return;
		}

		// Set Owner of Blink Widget to Local Player Controller
		if (APlayerController* AvatarPlayerController = Cast<APlayerController>(AvatarCharacter->GetController()))
		{
			if (AvatarPlayerController->IsLocalController())
			{
				// Create Blink Widget and Add to Viewport in Desired Size
				UTracerBlinkWidget* TracerBlinkWidget = CreateWidget<UTracerBlinkWidget>(AvatarPlayerController, TracerBlinkWidgetClass); 
				if (TracerBlinkWidget)
				{
					int32 ViewWidth, ViewHeight; 
					AvatarPlayerController->GetViewportSize(ViewWidth, ViewHeight); 
					TracerBlinkWidget->AddToPlayerScreen(10);
					TracerBlinkWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f)); 
					TracerBlinkWidget->SetDesiredSizeInViewport(FVector2D(100.f, 100.f)); 
					TracerBlinkWidget->SetPositionInViewport(FVector2D(ViewWidth * 0.5f, ViewHeight * 0.5f + 50.f), true); 
				}
			}
		}
	}
}
