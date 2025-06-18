// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncLoadPrimaryAsset.h"
#include "Engine/AssetManager.h"

void UAsyncLoadPrimaryAssetBase::Activate()
{
	check(UAssetManager::IsInitialized()); 
	UAssetManager& AssetManager = UAssetManager::Get(); 

	switch (Process)
	{
	case EAssetManagerProcess::Load:
		LoadHandle = AssetManager.LoadPrimaryAssets(AssetsToLoad, LoadBundles);
		break;
	case EAssetManagerProcess::ChangeBundleStateMatching:
		LoadHandle = AssetManager.ChangeBundleStateForMatchingPrimaryAssets(LoadBundles, OldBundles); 
		break;
	case EAssetManagerProcess::ChangeBundleStateList:
		LoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(AssetsToLoad, LoadBundles, OldBundles); 
		break;
	}

	if (LoadHandle.IsValid())
	{
		if (!LoadHandle->HasLoadCompleted())
		{
			LoadHandle->BindCompleteDelegate(FStreamableDelegate::CreateUObject(this, &UAsyncLoadPrimaryAssetBase::HandleLoadCompleted)); 
			return;
		}
	}

	HandleLoadCompleted(); 
}

void UAsyncLoadPrimaryAssetBase::HandleLoadCompleted()
{
	LoadHandle.Reset(); 
	SetReadyToDestroy(); 
}

void UAsyncLoadPrimaryAssetBase::GetCurrentlyLoadedAssets(TArray<UObject*>& AssetList)
{
	check(UAssetManager::IsInitialized()); 
	UAssetManager& AssetManager = UAssetManager::Get(); 
	// The assets may have already been loaded, but the handle was invalid, check the original list. 
	for (const FPrimaryAssetId& IdToLoad : AssetsToLoad)
	{
		UObject* LoadedObject = AssetManager.GetPrimaryAssetObject(IdToLoad); 
		if (LoadedObject)
		{
			AssetList.Add(LoadedObject); 
		}
	}
}

UAsyncLoadPrimaryAsset* UAsyncLoadPrimaryAsset::AsyncLoadPrimaryAsset(UObject* WorldContextObject, FPrimaryAssetId PrimaryAsset, const TArray<FName>& LoadBundles)
{
	UAsyncLoadPrimaryAsset* Action = NewObject<UAsyncLoadPrimaryAsset>(); 
	Action->AssetsToLoad.Add(PrimaryAsset); 
	Action->LoadBundles = LoadBundles; 
	Action->Process = EAssetManagerProcess::Load; 
	Action->RegisterWithGameInstance(WorldContextObject); 

	return Action; 
}

void UAsyncLoadPrimaryAsset::HandleLoadCompleted()
{
	UObject* LoadedAsset = nullptr; 
	TArray<UObject*> AssetList; 

	GetCurrentlyLoadedAssets(AssetList); 

	if (AssetList.Num() > 0)
	{
		LoadedAsset = AssetList[0];
	}

	Super::HandleLoadCompleted(); 
	Completed.Broadcast(LoadedAsset); 
}



