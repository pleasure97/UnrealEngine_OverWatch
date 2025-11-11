// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteractiveAPIData.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInteractiveAPIData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FString GetAPIModel();

	FString GetAPIUrl();

	FString GetAPIKey();

protected:
	UPROPERTY(EditDefaultsOnly)
	FString APIModel;

	UPROPERTY(EditDefaultsOnly)
	FString APIUrl;

	UPROPERTY(EditDefaultsOnly)
	FString APIKey;
};
