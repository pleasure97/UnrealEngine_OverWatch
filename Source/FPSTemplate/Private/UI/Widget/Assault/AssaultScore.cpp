// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultScore.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UAssaultScore::UpdateMatchDesign(bool bOffense, FLinearColor TeamColor)
{
	float Percent = bOffense ? 1.f : 0.1f;

	if (ProgressBar_AssaultScore)
	{
		ProgressBar_AssaultScore->SetFillColorAndOpacity(TeamColor);
		ProgressBar_AssaultScore->SetPercent(Percent);
	}
}

void UAssaultScore::UpdateScore()
{
	if (TextBlock_AssaultScore)
	{
		++AssaultScore; 
		TextBlock_AssaultScore->SetText(FText::AsNumber(AssaultScore)); 
	}
}
