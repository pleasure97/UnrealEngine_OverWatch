// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UltimateGauge.h"
#include "Components/Image.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UUltimateGauge::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	Percent = 0.f; 
	Image_UltimateIcon->SetRenderOpacity(0.f); 
}

void UUltimateGauge::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController);
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UUltimateGauge::ReceiveAbilityInfo);
		OverlayWidgetController->OnMaxUltimateGaugeChanged.AddDynamic(this, &UUltimateGauge::SetMaxUltimateGauge); 
		OverlayWidgetController->OnUltimateGaugeChanged.AddDynamic(this, &UUltimateGauge::UpdateUltimateGauge); 
	}

	if (Image_UltimateGauge)
	{
		UMaterialInterface* Material = Cast<UMaterialInterface>(Image_UltimateGauge->Brush.GetResourceObject()); 
		if (Material)
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this); 
			if (DynamicMaterialInstance)
			{
				DynamicMaterialInstance->SetScalarParameterValue(TEXT("Percent"), Percent);
				DynamicMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"),UltimateGaugeColor::Orange);
				Image_UltimateGauge->SetBrushFromMaterial(DynamicMaterialInstance); 
			}
		}
	}
}

void UUltimateGauge::ReceiveAbilityInfo(const FOWAbilityInfo& Info)
{
	if (!Info.InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag.Ultimate"))) { return; }
	
	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(Info.Icon.Get()));
	Image_UltimateIcon->SetBrush(SlateBrush); 
}

void UUltimateGauge::SetMaxUltimateGauge(float NewValue)
{
	MaxUltimateGauge = NewValue; 
}

void UUltimateGauge::UpdateUltimateGauge(float NewValue)
{
	if (NewValue >= MaxUltimateGauge)
	{
		if (bAlreadyUpdated) return;

		DynamicMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), UltimateGaugeColor::Blue);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("NumSections"), 0.f);
		Image_UltimateIcon->SetRenderOpacity(1.f);
		TextBlock_NumGauge->SetVisibility(ESlateVisibility::Collapsed); 
		TextBlock_Percent->SetVisibility(ESlateVisibility::Collapsed); 
		bAlreadyUpdated = true;
	}
	else
	{
		CurrentUltimateGauge = NewValue / MaxUltimateGauge;
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("Percent"), Percent);
		// TODO - Fix redundant function call 
		TextBlock_NumGauge->SetVisibility(ESlateVisibility::Visible);
		TextBlock_Percent->SetVisibility(ESlateVisibility::Visible);
		TextBlock_NumGauge->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(CurrentUltimateGauge))); 
	}
}
