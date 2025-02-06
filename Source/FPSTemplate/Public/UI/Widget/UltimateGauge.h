// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "UltimateGauge.generated.h"

class UImage; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UUltimateGauge : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateGauge; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Percent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateIcon;
};
