// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AssaultTimer.generated.h"

class UBorder; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAssaultTimer : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Timer; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_AssaultProgress; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AssaultProgress;

	void UpdateTeamColor(FLinearColor TeamColor);

	void UpdateCountdown(float CountdownLeft);

	void UpdateAssaultProgress(float InAssaultProgress); 
};
