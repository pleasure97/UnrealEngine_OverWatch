// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWWidgetLibrary.generated.h"

struct FWidgetControllerParams;
class AOWHUD;
class UTextBlock;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWWidgetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	 * Widget Controller
	 */

	UFUNCTION(BlueprintCallable, Category = "OWWidgetLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD);

	UFUNCTION(BlueprintCallable, Category = "OWWidgetLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	
	/*
	 * Widget Design 
	 */
	UFUNCTION(BlueprintCallable, Category = "OWWidgetLibrary|Widget Design")
	static void UpdatePureNumberText(UTextBlock* TextBlock, float TextBlockValue);
};
