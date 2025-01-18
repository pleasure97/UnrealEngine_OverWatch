// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWAbilitySystemLibrary.generated.h"

struct FWidgetControllerParams; 
class AOWHUD; 
class UOverlayWidgetController; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*
	 * Widget Controller 
	 */

	UFUNCTION(BlueprintCallable, Category="OWAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD); 
	
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject); 
};
