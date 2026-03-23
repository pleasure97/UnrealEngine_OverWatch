// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectStringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"

void UListDataObjectStringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;

	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B) -> bool
		{
			if (A.X != B.X)
			{
				return A.X <= B.X;
			}
			return A.Y <= B.Y;
		});

	for (const FIntPoint& AvailableResolution : AvailableResolutions)
	{
		AddDynamicOption(ResolutionToValueString(AvailableResolution), ResolutionToDisplayText(AvailableResolution));
	}

	MaximumAllowedResolution = ResolutionToValueString(AvailableResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObjectStringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResolutionToDisplayText(UOWGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObjectStringResolution::ResolutionToValueString(const FIntPoint& InResolution) const
{
	// Resolution Value from Dynamic Getter (X = 1920, Y = 1080)
	return FString::Printf(TEXT("(X=%i, Y=%i"), InResolution.X, InResolution.Y);
}

FText UListDataObjectStringResolution::ResolutionToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i X %i"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
