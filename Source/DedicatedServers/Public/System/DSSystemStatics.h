// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DSSystemStatics.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSSystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void PlayNextGame(const UObject* WorldContextObject); 
};
