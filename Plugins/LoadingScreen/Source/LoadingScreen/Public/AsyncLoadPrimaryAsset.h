// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/ObjectMacros.h"
#include "Engine/StreamableManager.h"
#include "Templates/SubclassOf.h"
#include "AsyncLoadPrimaryAsset.generated.h"

/**
 * Base Class of All Asset Manager Load Calls 
 */
UCLASS(Abstract)
class LOADINGSCREEN_API UAsyncLoadPrimaryAssetBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// Execute the actual load 
	virtual void Activate() override; 
	
protected:
	// Called from asset manager
	virtual void HandleLoadCompleted(); 

	// Gets all UObjects matching AssetsToLoad that are already in memory 
	virtual void GetCurrentlyLoadedAssets(TArray<UObject*>& AssetList); 

	// Specific Assets requested
	TArray<FPrimaryAssetId> AssetsToLoad; 

	// Bundle states
	TArray<FName> LoadBundles; 

	// Old Bundle States
	TArray<FName> OldBundles; 

	// Handle of Load Request
	TSharedPtr<FStreamableHandle> LoadHandle; 

	enum class EAssetManagerProcess: uint8
	{
		Load, 
		ChangeBundleStateMatching, 
		ChangeBundleStateList
	};

	// Which process is being run 
	EAssetManagerProcess Process; 
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryAssetLoadedDelegate, UObject*, Loaded);

UCLASS()
class LOADINGSCREEN_API UAsyncLoadPrimaryAsset :public UAsyncLoadPrimaryAssetBase
{
	GENERATED_BODY()

public:
	// Load a primary asset object into memory, this will cause it to stay loaded until it is explicitly unloaded.
	// The completed event will happen when when the load succeeds or fails, you should cast the loaded object to verify it is the correct type. 
	// If LoadBundles is specified, those bundles are loaded along with the asset. 
	UFUNCTION(BlueprintCallable, meta=(Category = "AssetManager", AutoCreateRefTerm = "LoadBundles", WorldContext = "WorldContextObject"))
	static UAsyncLoadPrimaryAsset* AsyncLoadPrimaryAsset(UObject* WorldContextObject, FPrimaryAssetId PrimaryAsset, const TArray<FName>& LoadBundles); 

	UPROPERTY(BlueprintAssignable)
	FOnPrimaryAssetLoadedDelegate Completed;

protected:
	// Called from Asset Manager
	virtual void HandleLoadCompleted() override;
};
