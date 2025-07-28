// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AssaultScoreDetails.generated.h"

class UBorder; 
class UTextBlock; 
class UTimerWidget; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAssaultScoreDetails : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Timer; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_AssaultProgress; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_RemainingTime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AssaultProgress;

	void UpdateTeamColor(FLinearColor TeamColor);

	void UpdateAssaultProgress(float InAssaultProgress); 

	void UpdateRemainingTime(FText RemainingTimeText); 
};
