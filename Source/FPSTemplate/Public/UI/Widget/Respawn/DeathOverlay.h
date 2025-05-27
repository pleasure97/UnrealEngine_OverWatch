// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "DeathOverlay.generated.h"

class UTextBlock; 
class URespawnGauge; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UDeathOverlay : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_GameTime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URespawnGauge> WBP_RespawnGauge;
	
	
};
