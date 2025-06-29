// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Assault/AssaultScore.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UAssaultScore::UpdateOffenseDesign(FLinearColor TeamColor)
{
	if (ProgressBar_AssaultScore)
	{
		ProgressBar_AssaultScore->SetFillColorAndOpacity(TeamColor); 
		ProgressBar_AssaultScore->SetPercent(1.f); 
	}
}

void UAssaultScore::UpdateDefenseDesign(FLinearColor TeamColor)
{
	if (ProgressBar_AssaultScore)
	{
		ProgressBar_AssaultScore->SetFillColorAndOpacity(TeamColor);
		ProgressBar_AssaultScore->SetPercent(0.1f);
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
