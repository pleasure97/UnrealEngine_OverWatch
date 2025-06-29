// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultTimer.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UAssaultTimer::UpdateTeamColor(FLinearColor TeamColor)
{
	if (Border_AssaultProgress)
	{
		Border_AssaultProgress->SetBrushColor(TeamColor);
	}
	if (Border_Timer)
	{
		Border_Timer->SetBrushColor(TeamColor);
	}
}

void UAssaultTimer::UpdateCountdown(float CountdownLeft)
{
	if (TextBlock_Time)
	{
		const int32 TotalSeconds = FMath::Max(0, FMath::FloorToInt(CountdownLeft));

		const int32 MinutesLeft = TotalSeconds / 60;
		const int32 SecondsLeft = TotalSeconds % 60;

		const FString TimeString = FString::Printf(TEXT("%d:%02d"), MinutesLeft, SecondsLeft);

		TextBlock_Time->SetText(FText::FromString(TimeString)); 
	}
}

void UAssaultTimer::UpdateAssaultProgress(float InAssaultProgress)
{
	if (TextBlock_AssaultProgress)
	{
		const float AssaultProgressPercent = InAssaultProgress * 100.f; 
		const FString AssaultProgressString = FString::Printf(TEXT("%.2f"), AssaultProgressPercent); 

		TextBlock_AssaultProgress->SetText(FText::FromString(AssaultProgressString));
	}
}
