// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Heroes/TracerBlinkWidget.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OWGameplayTags.h"
#include "Components/ProgressBar.h"

void UTracerBlinkWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(BlinkAbilityClass); 

	// Get Owning Player Pawn, Cast it to Ability System Interface, and Get Ability System Component 
	if (APawn* OwningPawn = Cast<APawn>(GetOwningPlayerPawn()))
	{
		if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwningPawn))
		{
			OwnerAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent(); 
		}
	}

	// Check if Owner Ability System Component is Valid 
	if (!IsValid(OwnerAbilitySystemComponent))
	{
		return;
	}

	// Iterate Activatable Abilities of Owner Ability System Component 
	for (const FGameplayAbilitySpec& GameplayAbilitySpec : OwnerAbilitySystemComponent->GetActivatableAbilities())
	{
		// Check if Gameplay Ability Class is Blink Ability Class 
		if (GameplayAbilitySpec.Ability && (GameplayAbilitySpec.Ability->GetClass() == BlinkAbilityClass))
		{
			// Get Cooldown Gameplay Effect
			UGameplayEffect* CooldownGameplayEffect = GameplayAbilitySpec.Ability->GetCooldownGameplayEffect(); 
			// Register Gameplay Tag Event when Cooldown Tag Count Changed 
			if (CooldownGameplayEffect)
			{
				OwnerAbilitySystemComponent->RegisterGameplayTagEvent(
					FOWGameplayTags::Get().Cooldown_Tracer_Blink, EGameplayTagEventType::AnyCountChange)
					.AddUObject(this, &UTracerBlinkWidget::OnBlinkStackChanged); 

			}
			break;
		}
	}
}

void UTracerBlinkWidget::NativeDestruct()
{
	// Unregister Gameplay Tag Event when Cooldown Tag Count Changed 
	if (IsValid(OwnerAbilitySystemComponent))
	{
		OwnerAbilitySystemComponent->RegisterGameplayTagEvent(
			FOWGameplayTags::Get().Cooldown_Tracer_Blink, EGameplayTagEventType::AnyCountChange)
			.RemoveAll(this); 
	}

	Super::NativeDestruct(); 
}

void UTracerBlinkWidget::OnBlinkStackChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	// Check if CooldownTag is Blink Cooldown Tag 
	if (CooldownTag != FOWGameplayTags::Get().Cooldown_Tracer_Blink)
	{
		return;
	}

	// Set Percent Literally 
	switch (NewCount)
	{
	case 0:
	{
		if (ProgressBar_Blink)
		{
			ProgressBar_Blink->SetPercent(0.f); 
		}
		break;
	}
	case 1:
	{
		if (ProgressBar_Blink)
		{
			ProgressBar_Blink->SetPercent(0.372f); 
		}
		break;
	}
	case 2:
	{
		if (ProgressBar_Blink)
		{
			ProgressBar_Blink->SetPercent(0.64f);
		}
		break;
	}
	case 3:
	{
		if (ProgressBar_Blink)
		{
			ProgressBar_Blink->SetPercent(1.f);
		}
		break;
	}
	}
}
