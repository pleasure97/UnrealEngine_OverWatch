// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingBackgroundActor.generated.h"

class ULoadingTask; 
class UAsyncLoadPrimaryAsset; 
class ULevelStreamingDynamic; 

UCLASS()
class LOADINGSCREEN_API ALoadingBackgroundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALoadingBackgroundActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType PrimaryAssetType; 	

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY()
	TObjectPtr<ULoadingTask> LoadingTask;

	UPROPERTY()
	TObjectPtr<UAsyncLoadPrimaryAsset> AsyncLoadPrimaryAsset;

	UPROPERTY()
	TObjectPtr<ULevelStreamingDynamic> LevelStreamingDynamic;

	void ShowLoadingScreen(); 

	void HideLoadingScreen(); 

	UFUNCTION()
	void OnLoadingCompleted(UObject* Loaded); 

	UFUNCTION()
	void OnLevelLoaded(); 

	UFUNCTION()
	void OnLevelShown(); 
};
