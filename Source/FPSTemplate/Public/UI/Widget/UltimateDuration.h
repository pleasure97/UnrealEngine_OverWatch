// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "UltimateDuration.generated.h"

class UImage; 
class UTextBlock; 
class UMaterialInstanceDynamic; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UUltimateDuration : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateDuration; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UltimateDuration; 

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance; 

	UPROPERTY()
	float Duration = 5.2f; 

protected:
	virtual void NativePreConstruct() override; 

	void UpdateDurationText(float RemainingTime); 

	void UpdateDurationBar(float RemainingTime); 
};
