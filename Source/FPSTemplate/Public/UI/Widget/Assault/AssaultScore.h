// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AssaultScore.generated.h"

class UProgressBar; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAssaultScore : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_AssaultScore; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_AssaultScore; 

	void UpdateOffenseDesign(FLinearColor TeamColor);

	void UpdateDefenseDesign(FLinearColor TeamColor);

	void UpdateScore(); 

private:
	int32 AssaultScore = 0; 
};
