// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UltimateGauge.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Components/TextBlock.h"

void UUltimateGauge::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	// Initialize Percent to 0 and Make Ultimate Icon not visible 
	Percent = 0.f; 

	if (Border_UltimateIcon)
	{
		Border_UltimateIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Image_UltimateIcon)
	{
		Image_UltimateIcon->SetVisibility(ESlateVisibility::Collapsed); 
	}

	if (Image_UltimateRay)
	{
		Image_UltimateRay->SetVisibility(ESlateVisibility::Collapsed); 
	}
}

void UUltimateGauge::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	// Cast Widget Controller to Overlay Widget Controller 
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		// Bind Overlay Widget Controller's Delegates
		SetWidgetController(OverlayWidgetController);
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UUltimateGauge::ReceiveAbilityInfo);
		OverlayWidgetController->OnMaxUltimateGaugeChanged.AddDynamic(this, &UUltimateGauge::SetMaxUltimateGauge); 
		OverlayWidgetController->OnUltimateGaugeChanged.AddDynamic(this, &UUltimateGauge::UpdateUltimateGauge); 
	}

	// Initialize Ultimate Gauge's Material Instance Dynamic 
	if (Image_UltimateGauge)
	{
		UltimateGaugeMID = Image_UltimateGauge->GetDynamicMaterial(); 

		if (UltimateGaugeMID)
		{
			UltimateGaugeMID->SetScalarParameterValue(TEXT("Percent"), Percent);
			UltimateGaugeMID->SetVectorParameterValue(TEXT("BaseColor"), OrangeColor);
			UltimateGaugeMID->SetScalarParameterValue(TEXT("NumSections"), 100.f);

			Image_UltimateGauge->SetBrushFromMaterial(UltimateGaugeMID);
		}
	}
}

void UUltimateGauge::NativeDestruct()
{
	// Cast Widget Controller to Overlay Widget Controller 
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		// Remove Overlay Widget Controller's Bindings
		OverlayWidgetController->AbilityInfoDelegate.RemoveAll(this); 
		OverlayWidgetController->OnMaxUltimateGaugeChanged.RemoveAll(this);
		OverlayWidgetController->OnUltimateGaugeChanged.RemoveAll(this);
	}

	Super::NativeDestruct(); 
}

void UUltimateGauge::ReceiveAbilityInfo(const FOWAbilityInfo& Info)
{
	if (Info.InputTag.MatchesTag(FOWGameplayTags::Get().InputTag_Ultimate))
	{
		if (Image_UltimateIcon)
		{
			FSlateBrush SlateBrush;
			SlateBrush.SetResourceObject(const_cast<UTexture2D*>(Info.Icon.Get()));
			Image_UltimateIcon->SetBrush(SlateBrush);
		}
	}
}

void UUltimateGauge::SetMaxUltimateGauge(float NewValue)
{
	MaxUltimateGauge = NewValue; 
}

void UUltimateGauge::UpdateUltimateGauge(float NewValue)
{
	// Check if Max Ultimate Gauge is Broadcasted 
	if (MaxUltimateGauge == 0)
	{
		return;
	}

	// Early return When the difference between the new value and the current ultimate gauge does not exceed 1%
	/*if (FMath::Abs(NewValue - CurrentUltimateGauge) < MaxUltimateGauge * 0.01f)
	{
		return;
	}*/
	
	if (NewValue >= MaxUltimateGauge)
	{
		// Early return if Ultimate is already charged 
		if (bAlreadyUpdated)
		{
			return;
		}

		// TODO - Widget Animation?
		
		if (UltimateGaugeMID)
		{
			UltimateGaugeMID->SetVectorParameterValue(TEXT("BaseColor"), SkyColor);
			UltimateGaugeMID->SetScalarParameterValue(TEXT("NumSections"), 0.f);
		}

		if (Border_UltimateIcon)
		{
			Border_UltimateIcon->SetVisibility(ESlateVisibility::Visible); 
		}
		
		if (Image_UltimateIcon)
		{
			Image_UltimateIcon->SetVisibility(ESlateVisibility::Visible);
		}

		if (TextBlock_NumGauge)
		{
			TextBlock_NumGauge->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		if (TextBlock_Percent)
		{
			TextBlock_Percent->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (Image_UltimateRay)
		{
			Image_UltimateRay->SetVisibility(ESlateVisibility::Visible); 
		}

		bAlreadyUpdated = true;
	}
	else
	{
		Percent = NewValue / MaxUltimateGauge;
		// After Casting Ultimate, Current Ultimate Gauge is greater than New Value 
		if (CurrentUltimateGauge > NewValue)
		{
			if (TextBlock_NumGauge)
			{
				TextBlock_NumGauge->SetText(FText::AsNumber(FMath::TruncToInt(NewValue / MaxUltimateGauge * 100.f)));
				TextBlock_NumGauge->SetVisibility(ESlateVisibility::Visible);
			}

			if (TextBlock_Percent)
			{
				TextBlock_Percent->SetVisibility(ESlateVisibility::Visible);
			}

			if (UltimateGaugeMID)
			{
				UltimateGaugeMID->SetScalarParameterValue(TEXT("Percent"), Percent);
				UltimateGaugeMID->SetVectorParameterValue(TEXT("BaseColor"), OrangeColor);
				UltimateGaugeMID->SetScalarParameterValue(TEXT("NumSections"), 100.f);
			}

			if (Border_UltimateIcon)
			{
				Border_UltimateIcon->SetVisibility(ESlateVisibility::Collapsed); 
			}

			if (Image_UltimateIcon)
			{
				Image_UltimateIcon->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (Image_UltimateRay)
			{
				Image_UltimateRay->SetVisibility(ESlateVisibility::Collapsed);
			}
			
			// Reset Ultimate Update Status
			bAlreadyUpdated = false; 
		}
		else
		{
			if (UltimateGaugeMID)
			{
				UltimateGaugeMID->SetScalarParameterValue(TEXT("Percent"), Percent);
			}

			if (TextBlock_NumGauge)
			{
				TextBlock_NumGauge->SetText(FText::AsNumber(FMath::TruncToInt(NewValue / MaxUltimateGauge * 100.f)));
			}
		}
	}

	CurrentUltimateGauge = NewValue;
}
