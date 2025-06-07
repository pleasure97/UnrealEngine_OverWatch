// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoadingInterface.h"
#include "LoadingTask.generated.h"

/**
 * 
 */
UCLASS()
class LOADINGSCREEN_API ULoadingTask : public UObject, public ILoadingInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext="WorldContextObject"))
	static ULoadingTask* CreateLoadingScreenTask(UObject* WorldContextObject, const FString& ShowLoadingScreenReason); 

public:
	ULoadingTask() {}

	UFUNCTION(BlueprintCallable)
	void Unregister(); 

	UFUNCTION(BlueprintCallable)
	void SetShowLoadingScreenReason(const FString& InReason); 

	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override; 

	FString Reason; 
};
