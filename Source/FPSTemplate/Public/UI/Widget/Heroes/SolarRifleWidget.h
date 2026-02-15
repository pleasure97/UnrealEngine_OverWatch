// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "SolarRifleWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API USolarRifleWidget : public UOWUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
};
