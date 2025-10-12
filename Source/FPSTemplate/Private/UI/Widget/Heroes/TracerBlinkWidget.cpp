// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Heroes/TracerBlinkWidget.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OWGameplayTags.h"
#include "Components/ProgressBar.h"
#include "AbilitySystem/OWAttributeSet.h"

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

		// Check if Owner Ability System Component is Valid 
		if (IsValid(OwnerAbilitySystemComponent))
		{
			// Iterate Activatable Abilities of Owner Ability System Component 
			for (const FGameplayAbilitySpec& GameplayAbilitySpec : OwnerAbilitySystemComponent->GetActivatableAbilities())
			{
				// Check if Gameplay Ability Class is Blink Ability Class 
				if (GameplayAbilitySpec.Ability && (GameplayAbilitySpec.Ability->GetClass() == BlinkAbilityClass))
				{
					if (UOWGameplayAbility* OWGameplayAbility = Cast<UOWGameplayAbility>(GameplayAbilitySpec.Ability))
					{
						// Get Current Stack Attribute from CDO 
						const FGameplayAttribute StackAttribute = OWGameplayAbility->CurrentStackAttribute;

						OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
							OWGameplayAbility->CurrentStackAttribute).AddLambda(
								[this, StackAttribute](const FOnAttributeChangeData& Data)
								{
									UpdateNumBlinks(StackAttribute, Data);
								}
							);
						break;
					}
				}
			}
		}
	}
}

void UTracerBlinkWidget::UpdateNumBlinks(const FGameplayAttribute& Attribute, const FOnAttributeChangeData& Data)
{
	// Set Percent Literally 
	switch (FMath::FloorToInt(Data.NewValue))
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


