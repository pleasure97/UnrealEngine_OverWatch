// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingBackgroundActor.generated.h"

class ULoadingTask; 

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
	
private:
	UPROPERTY()
	TObjectPtr<ULoadingTask> LoadingTask;

	void ShowLoadingScreen(); 

	void HideLoadingScreen(); 

	UFUNCTION()
	void OnLoadingCompleted(UObject* Loaded); 

	UFUNCTION()
	void OnLevelLoaded(); 

	UFUNCTION()
	void OnLevelShown(); 
};
