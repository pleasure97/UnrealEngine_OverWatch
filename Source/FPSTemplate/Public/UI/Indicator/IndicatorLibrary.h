// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IndicatorLibrary.generated.h"

class UIndicatorManagerComponent; 
class AController; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UIndicatorLibrary(); 

	UFUNCTION(BlueprintCallable, Category = "Indicator")
	static UIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller); 
};
