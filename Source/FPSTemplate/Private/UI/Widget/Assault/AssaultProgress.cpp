// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultProgress.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Team/OWTeamSubsystem.h"
#include "Game/OWGamePhaseSubsystem.h"
#include "OWGameplayTags.h"
#include "Animation/WidgetAnimation.h"

void UAssaultProgress::NativeConstruct()
{
	Super::NativeConstruct();

	// Save Material Instance Dynamic of Assault Progress
	if (Image_AssaultProgress)
	{
		AssaultProgressMID = Image_AssaultProgress->GetDynamicMaterial();
	}

	HideAssaultWidgets();

	// Get Team Subsystem and Set Owner Team ID 
	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
	if (TeamSubsystem)
	{
		OwnerTeamID = TeamSubsystem->FindTeamFromObject(GetOwningPlayer()); 
	}
}

void UAssaultProgress::HideAssaultWidgets()
{
	// Hide NumOccupied UI 
	if (HorizontalBox_NumOccupied)
	{
		HorizontalBox_NumOccupied->SetVisibility(ESlateVisibility::Hidden);
	}

	// Collapse Contesting UI 
	if (HorizontalBox_Contesting)
	{
		HorizontalBox_Contesting->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAssaultProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Interpolate Occupation Progress 
}

void UAssaultProgress::UpdateNumAttackers(int32 NumAttackers)
{
	if (NumAttackers == 0)
	{
		if (Border_NumOccupiedAttackers)
		{
			Border_NumOccupiedAttackers->SetVisibility(ESlateVisibility::Hidden); 
		}
	}
	else
	{
		if (Border_NumOccupiedAttackers)
		{
			if (Border_NumOccupiedAttackers->GetVisibility() != ESlateVisibility::Visible)
			{
				Border_NumOccupiedAttackers->SetVisibility(ESlateVisibility::Visible);
			}
		}
		
		if (TextBlock_NumOccupiedAttackers)
		{
			TextBlock_NumOccupiedAttackers->SetText(FText::AsNumber(NumAttackers)); 
		}
	}

	CurrentNumAttackers = NumAttackers; 
}

void UAssaultProgress::UpdateNumDefenders(int32 NumDefenders)
{
	if (NumDefenders == 0)
	{
		if (Border_NumOccupiedDefenders)
		{
			Border_NumOccupiedDefenders->SetVisibility(ESlateVisibility::Hidden); 
		}
	}
	else
	{
		if (CurrentNumAttackers <= 0)
		{
			return; 
		}

		if (Border_NumOccupiedDefenders)
		{
			if (Border_NumOccupiedAttackers->GetVisibility() != ESlateVisibility::Visible)
			{
				Border_NumOccupiedAttackers->SetVisibility(ESlateVisibility::Visible);
			}
		}

		if (TextBlock_NumOccupiedDefenders)
		{
			TextBlock_NumOccupiedAttackers->SetText(FText::AsNumber(NumDefenders)); 
		}
	}
}

void UAssaultProgress::UpdateOccupationProgress(float OccupationProgress)
{
	if (AssaultProgressMID)
	{
		AssaultProgressMID->SetScalarParameterValue(TEXT("Percent"), OccupationProgress); 
	}
}

void UAssaultProgress::UpdateOccupationState(EOccupationState OccupationState)
{
	switch (OccupationState)
	{
	case EOccupationState::None:
	{
		UE_LOG(LogTemp, Log, TEXT("Occupation State is None in AssaultProgress UI"));
		break;
	}
	case EOccupationState::Start:
	{
		// Set Num Occupied Visibility
		if (HorizontalBox_NumOccupied && (HorizontalBox_NumOccupied->GetVisibility() == ESlateVisibility::Visible))
		{
			HorizontalBox_NumOccupied->SetVisibility(ESlateVisibility::Visible); 
		}

		StopContestingAnimation();
		break;
	}
	case EOccupationState::Contesting:
	{
		// Set Num Occupied Visibility
		if (HorizontalBox_NumOccupied && (HorizontalBox_NumOccupied->GetVisibility() != ESlateVisibility::Visible))
		{
			HorizontalBox_NumOccupied->SetVisibility(ESlateVisibility::Visible);
		}

		PlayContestingAnimation();
		break;
	}
	case EOccupationState::Stop:
	{
		StopContestingAnimation();

		// Set Num Occupied Visibility
		if (HorizontalBox_NumOccupied && (HorizontalBox_NumOccupied->GetVisibility() != ESlateVisibility::Visible))
		{
			HorizontalBox_NumOccupied->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	}
	case EOccupationState::Complete:
	{
		HideAssaultWidgets(); 

		// TODO - Check Mark or End Phase?
		break;
	}
	}
}

void UAssaultProgress::PlayContestingAnimation()
{
	// Play Widget Animation 
	if (HorizontalBox_Contesting && Contesting)
	{
		HorizontalBox_Contesting->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Contesting, 0.f, 0);
	}
}

void UAssaultProgress::StopContestingAnimation()
{
	// Stop Widget Animation 
	if (HorizontalBox_Contesting && Contesting && IsAnimationPlaying(Contesting))
	{
		StopAnimation(Contesting);
		HorizontalBox_Contesting->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAssaultProgress::UpdateProgressDesign(int32 OffenseTeamID)
{
	if (OwnerTeamID <= 0 || OffenseTeamID <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner Team ID is not initialized or Offense Team ID is not valid."));
		return; 
	}

	if (OwnerTeamID == OffenseTeamID)
	{
		if (Border_NumOccupiedAttackers)
		{
			Border_NumOccupiedAttackers->SetBrushColor(BlueTeamColor); 
		}
		if (Border_NumOccupiedDefenders)
		{
			Border_NumOccupiedDefenders->SetBrushColor(RedTeamColor); 
		}
		if (Image_AssaultRhombus)
		{
			Image_AssaultRhombus->SetBrushFromTexture(RedAssaultRhombus, true); 
		}
	}
	else
	{
		if (Border_NumOccupiedAttackers)
		{
			Border_NumOccupiedAttackers->SetBrushColor(RedTeamColor);
		}
		if (Border_NumOccupiedDefenders)
		{
			Border_NumOccupiedDefenders->SetBrushColor(BlueTeamColor);
		}
		if (Image_AssaultRhombus)
		{
			Image_AssaultRhombus->SetBrushFromTexture(BlueAssaultRhombus, true);
		}
	}
}

