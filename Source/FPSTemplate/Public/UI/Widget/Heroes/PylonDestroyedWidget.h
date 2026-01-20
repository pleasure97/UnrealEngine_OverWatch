// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "PylonDestroyedWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPylonDestroyedWidget : public UOWUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeWidgetDestroyed = 3.f;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void WidgetDestroyed();
};
