// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "RespawnGauge.generated.h"

class UImage; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API URespawnGauge : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_RespawnGauge; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_RespawnTime; 
};
