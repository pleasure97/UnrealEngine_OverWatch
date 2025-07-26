// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingBackgroundActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LoadingTask.h"
#include "Engine/LevelStreamingDynamic.h"
#include "AsyncLoadPrimaryAsset.h"
#include "LoadingPrimaryDataAsset.h"

ALoadingBackgroundActor::ALoadingBackgroundActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoadingBackgroundActor::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<FPrimaryAssetId> LoadingBackgroundArray; 
	UKismetSystemLibrary::GetPrimaryAssetIdList(PrimaryAssetType, LoadingBackgroundArray); 

	// Assume there is only 1 Primary Asset ID corresponding to the relevant Primary Asset Type. 
	if (LoadingBackgroundArray.Num() > 0)
	{
		for (FPrimaryAssetId& LoadingBackground : LoadingBackgroundArray)
		{
			ShowLoadingScreen(); 
			TArray<FName> LoadedBundles; 
			AsyncLoadPrimaryAsset = UAsyncLoadPrimaryAsset::AsyncLoadPrimaryAsset(this, LoadingBackground, LoadedBundles); 
			AsyncLoadPrimaryAsset->Completed.AddDynamic(this, &ALoadingBackgroundActor::OnLoadingCompleted); 
			AsyncLoadPrimaryAsset->Activate(); 
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Backgrounds Found!")); 
	}
}

void ALoadingBackgroundActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AsyncLoadPrimaryAsset)
	{
		AsyncLoadPrimaryAsset->Completed.RemoveAll(this); 
	}

	if (LevelStreamingDynamic)
	{
		LevelStreamingDynamic->OnLevelLoaded.RemoveAll(this); 
		LevelStreamingDynamic->OnLevelShown.RemoveAll(this); 
	}

	Super::EndPlay(EndPlayReason); 
}

void ALoadingBackgroundActor::ShowLoadingScreen()
{
	FString LoadingScreenReason = "Loading Background"; 
	LoadingTask = ULoadingTask::CreateLoadingScreenTask(this, LoadingScreenReason); 
}

void ALoadingBackgroundActor::HideLoadingScreen()
{
	FTimerHandle TimerHandle; 
	float DelayTime = 0.5f; 

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (LoadingTask)
			{
				LoadingTask->Unregister(); 
			}

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle); 
		}), DelayTime, false); 
}

void ALoadingBackgroundActor::OnLoadingCompleted(UObject* Loaded)
{
	if (ULoadingPrimaryDataAsset* LoadingPrimaryDataAsset = Cast<ULoadingPrimaryDataAsset>(Loaded))
	{
		bool bSuccess; 
		LevelStreamingDynamic = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			this, 
			LoadingPrimaryDataAsset->LoadingLevel,
			GetActorLocation(), 
			GetActorRotation(),
			bSuccess); 

		if (bSuccess && LevelStreamingDynamic)
		{
			LevelStreamingDynamic->OnLevelLoaded.AddDynamic(this, &ALoadingBackgroundActor::OnLevelLoaded); 
			LevelStreamingDynamic->OnLevelShown.AddDynamic(this, &ALoadingBackgroundActor::OnLevelShown); 
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Loading Level Instance failed."));
			HideLoadingScreen(); 
		}
	}
	else
	{
		HideLoadingScreen(); 
	}
}

void ALoadingBackgroundActor::OnLevelLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("Level is loaded.")); 
}

void ALoadingBackgroundActor::OnLevelShown()
{
	UE_LOG(LogTemp, Log, TEXT("Level is shown.")); 

	HideLoadingScreen(); 
}
