// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UltimateDuration.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"

void UUltimateDuration::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	if (Image_UltimateDuration)
	{
		UMaterialInterface* Material = Cast<UMaterialInterface>(Image_UltimateDuration->GetBrush().GetResourceObject()); 
		if (Material)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this); 
			if (DynamicMaterialInstance)
			{
				DynamicMaterialInstance->SetScalarParameterValue(TEXT("Percent"), 1.f); 
				Image_UltimateDuration->SetBrushFromMaterial(DynamicMaterialInstance);
			}
		}
	}
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
