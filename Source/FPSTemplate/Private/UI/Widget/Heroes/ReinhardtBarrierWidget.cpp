// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Heroes/ReinhardtBarrierWidget.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Actor/EnergyBarrier.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UReinhardtBarrierWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Save Material Instance Dynamic of Barrier Field Image Widget 
	if (Image_BarrierField)
	{
		MID_BarrierField = Image_BarrierField->GetDynamicMaterial();
	}

	// Check if Owning Player Pawn is Valid 
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!IsValid(OwningPawn))
	{
		return;
	}
	
	// Find Attached Actors of Owning Player Pawn 
	TArray<AActor*> AttachedActors; 
	OwningPawn->GetAttachedActors(AttachedActors); 

	// Iterate Attached Actors 
	for (AActor*& AttachedActor : AttachedActors)
	{
		// Cast Attached Actor to Energy Barrier 
		if (AEnergyBarrier* EnergyBarrier = Cast<AEnergyBarrier>(AttachedActor); IsValid(EnergyBarrier))
		{
			// Save Barrier Ability System Component as Weak Object Pointer
			BarrierASC = EnergyBarrier->GetAbilitySystemComponent(); 
			if (!IsValid(BarrierASC.Get()))
			{
				UE_LOG(LogTemp, Error, TEXT("Energy Barrier ASC is Not Valid in UReinhardtBarrierWidget::NativeConstruct()"));
				return;
			}

			// Bind 'Health' Gameplay Attribute Value Changed Delegate of Barrier Ability System Component
			BarrierASC->GetGameplayAttributeValueChangeDelegate(
				UOWAttributeSet::GetHealthAttribute()).AddWeakLambda(
					this,
					[this](const FOnAttributeChangeData& Data)
					{
						UpdateBarrierHealth(Data.NewValue);
					}
				);

			// Bind 'Max Health' Gameplay Attribute Value Changed Delegate of Barrier Ability System Component
			BarrierASC->GetGameplayAttributeValueChangeDelegate(
				UOWAttributeSet::GetMaxHealthAttribute()).AddWeakLambda(
					this,
					[this](const FOnAttributeChangeData& Data)
					{
						UpdateBarrierMaxHealth(Data.NewValue);
					}
				);

			// Early Return 
			return;
		}
	}
}

void UReinhardtBarrierWidget::NativeDestruct()
{
	if (IsValid(BarrierASC.Get()))
	{
		BarrierASC->GetGameplayAttributeValueChangeDelegate(UOWAttributeSet::GetHealthAttribute()).RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UReinhardtBarrierWidget::UpdateBarrierHealth(float NewHealthValue)
{
	if (SavedMaxHealth < 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Saved Max Health is less than 0 in UReinhardtBarrierWidget::UpdateBarrierHealth()"));
		return;
	}

	// Since Barrier Health is 0, there is No Need to Show Widget Anymore 
	if (NewHealthValue <= 0.f)
	{
		SetVisibility(ESlateVisibility::Collapsed); 
		return;
	}

	// Show Reamining Barrier Health with Decimal Point Dropped 
	if (TextBlock_BarrierField)
	{
		TextBlock_BarrierField->SetText(FText::AsNumber(FMath::TruncToInt(NewHealthValue)));
	}
	
	if (MID_BarrierField)
	{
		float NumUnits = FMath::FloorToInt((SavedMaxHealth - NewHealthValue) / HealthUnit); 
		NumUnits = FMath::Clamp(NumUnits, 0, HealthUnit);
		MID_BarrierField->SetScalarParameterValue(FName("Rounds_Current"), NumUnits); 
	}
}

void UReinhardtBarrierWidget::UpdateBarrierMaxHealth(float NewMaxHealthValue)
{
	if (NewMaxHealthValue > 0.f)
	{
		SavedMaxHealth = NewMaxHealthValue;
	}
}

