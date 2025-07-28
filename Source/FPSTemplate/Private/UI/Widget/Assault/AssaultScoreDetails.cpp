// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultScoreDetails.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UAssaultScoreDetails::UpdateTeamColor(FLinearColor TeamColor)
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

void UAssaultScoreDetails::UpdateAssaultProgress(float InAssaultProgress)
{
	if (TextBlock_AssaultProgress)
	{
		const float AssaultProgressPercent = InAssaultProgress * 100.f; 
		const FString AssaultProgressString = FString::Printf(TEXT("%.2f"), AssaultProgressPercent); 

		TextBlock_AssaultProgress->SetText(FText::FromString(AssaultProgressString));
	}
}

void UAssaultScoreDetails::UpdateRemainingTime(FText RemainingTimeText)
{
	if (TextBlock_RemainingTime)
	{
		TextBlock_RemainingTime->SetText(RemainingTimeText); 
	}
}
