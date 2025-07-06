// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelection.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

void UHeroSelection::NativeConstruct()
{
	if (Button_HeroSelection)
	{
		// Button Visual Effect
		if (Border_HeroSelection)
		{
			Border_HeroSelection->SetBrushColor(HeroSelectionButtonColors::Gray);
		}
		if (Overlay_HeroSelection)
		{
			Overlay_HeroSelection->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
		}
		if (TextBlock_HeroName)
		{
			TextBlock_HeroName->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (Image_HeroSelected)
		{
			Image_HeroSelected->SetVisibility(ESlateVisibility::Collapsed); 
		}

		// Bind Each Button Delegate to Callback
		if (Button_HeroSelection)
		{
			Button_HeroSelection->OnClicked.AddDynamic(this, &UHeroSelection::OnHeroSelectionButtonClicked);
			Button_HeroSelection->OnHovered.AddDynamic(this, &UHeroSelection::OnHeroSelectionButtonHovered);
			Button_HeroSelection->OnUnhovered.AddDynamic(this, &UHeroSelection::OnHeroSelectionButtonUnhovered);
			Button_HeroSelection->OnReleased.AddDynamic(this, &UHeroSelection::OnHeroSelectionButtonReleased);
		}
	}
}

void UHeroSelection::NativeDestruct()
{
	if (Button_HeroSelection)
	{
		// Unbind Each Button Delegate
		Button_HeroSelection->OnClicked.RemoveAll(this);
		Button_HeroSelection->OnHovered.RemoveAll(this); 
		Button_HeroSelection->OnUnhovered.RemoveAll(this); 
		Button_HeroSelection->OnReleased.RemoveAll(this); 
	}

	Super::NativeDestruct(); 
}

void UHeroSelection::SetHeroInfo(EHeroName InHeroName)
{
	HeroName = InHeroName; 

	// Get Hero Info from Using Custom Ability System Library 
	FOWHeroInfo OWHeroInfo;
	UOWAbilitySystemLibrary::GetIndividualHeroInfo(this, HeroName, OWHeroInfo);

	// Set Hero Name Text
	FText HeroNameText = StaticEnum<EHeroName>()->GetDisplayNameTextByValue((int64)HeroName);
	if (TextBlock_HeroName)
	{
		TextBlock_HeroName->SetText(HeroNameText);
	}

	// Set Hero Portrait
	if (Image_HeroPortrait2D)
	{
		Image_HeroPortrait2D->SetBrushFromTexture(const_cast<UTexture2D*>(OWHeroInfo.HeroPortrait2D.Get()), true);
	}
}

void UHeroSelection::OnHeroSelectionButtonClicked()
{
	// Button Visual Effect
	if (Overlay_HeroSelection)
	{
		FWidgetTransform WidgetTransform = Overlay_HeroSelection->RenderTransform;
		WidgetTransform.Scale = FVector2D(1.1f, 1.1f);
		Overlay_HeroSelection->SetRenderTransform(WidgetTransform);
	}
	
	if (Border_HeroSelection)
	{
		Border_HeroSelection->SetBrushColor(HeroSelectionButtonColors::Orange);
	}

	if (Image_HeroSelected)
	{
		Image_HeroSelected->SetVisibility(ESlateVisibility::Visible); 
	}

	// Broadcast Hero Name 
	OnHeroSelected.Broadcast(HeroName); 
}

void UHeroSelection::OnHeroSelectionButtonHovered()
{
	if (Overlay_HeroSelection)
	{
		FWidgetTransform WidgetTransform = Overlay_HeroSelection->RenderTransform;
		WidgetTransform.Scale = FVector2D(1.1f, 1.1f);
		Overlay_HeroSelection->SetRenderTransform(WidgetTransform);
	}
	
	if (Border_HeroSelection)
	{
		Border_HeroSelection->SetBrushColor(HeroSelectionButtonColors::White);
	}
}

void UHeroSelection::OnHeroSelectionButtonUnhovered()
{
	if (Overlay_HeroSelection)
	{
		FWidgetTransform WidgetTransform = Overlay_HeroSelection->RenderTransform;
		WidgetTransform.Scale = FVector2D(1.f, 1.f);
		Overlay_HeroSelection->SetRenderTransform(WidgetTransform);
	}
	
	if (Border_HeroSelection)
	{
		Border_HeroSelection->SetBrushColor(HeroSelectionButtonColors::Gray);
	}
}

void UHeroSelection::OnHeroSelectionButtonReleased()
{
	if (Border_HeroSelection)
	{
		Border_HeroSelection->SetBrushColor(HeroSelectionButtonColors::Gray);
	}

	OnHeroUnselected.Broadcast(); 
}


