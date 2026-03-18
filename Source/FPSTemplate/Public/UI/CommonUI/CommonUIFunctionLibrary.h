// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CommonUIFunctionLibrary.generated.h"

class UActivatableBaseWidget;
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "CommonUI Function Library")
	static TSoftClassPtr<UActivatableBaseWidget> GetCommonUISoftWidgetClassByTag(
		UPARAM(meta = (Categories = "CommonUI.Widget"))FGameplayTag InWidgetTag);
};
