// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UltimateDuration.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"

void UUltimateDuration::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	// Set Dynamic Material Instnace of Circular Progress Bar to Image Type SubWidget 
	if (Image_UltimateDuration)
	{
		// Get Material Interface from Brush 
		UMaterialInterface* Material = Cast<UMaterialInterface>(Image_UltimateDuration->GetBrush().GetResourceObject()); 
		if (Material)
		{
			// Create Dynamic Material Instance 
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this); 
			// Set Parameter of Dynamic Parameter Instance so that Progress Bar is fully filled 
			if (DynamicMaterialInstance)
			{
				DynamicMaterialInstance->SetScalarParameterValue(TEXT("Percent"), 1.f); 
				Image_UltimateDuration->SetBrushFromMaterial(DynamicMaterialInstance);
			}
		}
	}
}

void UUltimateDuration::NativeDestruct()
{
	// Clear Duration Tick Timer from World Timer Manager 
	if (UWorld* World = GetWorld())
	{
		if (DurationTickTimerHandle.IsValid())
		{
			World->GetTimerManager().ClearTimer(DurationTickTimerHandle);
		}
	}

	Super::NativeDestruct(); 
}

void UUltimateDuration::UpdateDuration()
{
	// Calculate Duration 
	Duration -= DurationTick;

	// Clear Duration Tick Timer from World Timer Manager, Remove From Parent, and Early Return 
	if (Duration <= 0.f)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(DurationTickTimerHandle); 
		}
		RemoveFromParent(); 
		return; 
	}

	UpdateDurationText(Duration);
	UpdateDurationBar(Duration);
}

void UUltimateDuration::SetDuration(float InDuration)
{
	// Set Duration Tick Timer (e.g., Duration - 10.f / Duration Tick - 0.1f)
	// UI Updates Smoothly Even at 0.1 Seconds Interval Without Using Tick 
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DurationTickTimerHandle, this, &UUltimateDuration::UpdateDuration, DurationTick, true);
	}
}

float UUltimateDuration::GetDuration() const
{
	return Duration; 
}

void UUltimateDuration::UpdateDurationText(float RemainingTime)
{
	FText RemainingTimeText = UKismetTextLibrary::Conv_IntToText(FMath::TruncToInt(RemainingTime));

	TextBlock_UltimateDuration->SetText(RemainingTimeText); 
}

void UUltimateDuration::UpdateDurationBar(float RemainingTime)
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("Percent"), RemainingTime / Duration); 
	}
}
