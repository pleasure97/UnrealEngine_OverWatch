// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OWUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; 

	virtual void SetChildWidgetControllers();
};
