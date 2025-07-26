// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LoadingPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta = (DisplayName = "LoadingPrimaryDataAsset"))
class LOADINGSCREEN_API ULoadingPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> LoadingLevel; 
};
