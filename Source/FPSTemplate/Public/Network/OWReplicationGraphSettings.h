// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "OWReplicationGraphTypes.h"
#include "OWReplicationGraphSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, MinimalAPI)
class UOWReplicationGraphSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
	
public:
	UOWReplicationGraphSettings(); 

public:
	/* Replication Graph */
	UPROPERTY(config, EditAnywhere, Category = ReplicationGraph)
	bool bEnableReplicationGraph = false;

	UPROPERTY(config, EditAnywhere, Category = ReplicationGraph, meta = (MetaClass = "/Script/FPSTemplate.OWReplicationGraph"))
	FSoftClassPath DefaultReplicationGraphClass;

	/* Fast Shared Path */
	UPROPERTY(EditAnywhere, Category = FastSharedPath, meta = (ConsoleVariable = "OW.ReplicationGraph.EnableFastSharedPath"))
	bool bEnableFastSharedPath = true; 

	// How much bandwidth to use for FastShared movement updates. This is counted independently of the NetDriver's target bandwidth.
	UPROPERTY(EditAnywhere, Category = FastSharedPath, meta = (ForceUnits=Kilobytes, ConsoleVariable = "OW.ReplicationGraph.TargetKBytesSecFastSharedPath"))
	int32 TargetKBytesSecFastSharedPath = 10; 

	UPROPERTY(EditAnywhere, Category = FastSharedPath, meta = (ConsoleVariable = "OW.ReplicationGraph.FastSharedPathCullDistPercentage"))
	float FastSharedPathCullDistPercentage = 0.8f;

	/* Destruction Information */
	UPROPERTY(EditAnywhere, Category = DestructionInfo, meta = (ForceUnits = cm, ConsoleVariable = "OW.ReplicationGraph.DestructInfo.MaxDistance"))
	float DestructionInfoMaxDistance = 30000.f;

	/* Spatial Grid */
	UPROPERTY(EditAnywhere, Category = SpatialGrid, meta = (ForceUnits = cm, ConsoleVariable = "OW.ReplicationGraph.CellSize"))
	float SpatialGridCellSize = 10000.f;

	// Essentially "Min X" for replication. This is just an initial value. The system will reset itself if actors appears outside of this.
	UPROPERTY(EditAnywhere, Category = SpatialGrid, meta = (ForceUnits = cm, ConsoleVariable = "OW.ReplicationGraph.SpatialBiasX"))
	float SpatialBiasX = -200000.f;

	// Essentially "Min Y" for replication. This is just an initial value. The system will reset itself if actors appears outside of this.
	UPROPERTY(EditAnywhere, Category = SpatialGrid, meta = (ForceUnits = cm, ConsoleVariable = "OW.ReplicationGraph.SpatialBiasY"))
	float SpatialBiasY = -200000.0f;

	UPROPERTY(EditAnywhere, Category = SpatialGrid, meta = (ConsoleVariable = "OW.ReplicationGraph.EnableSpatialRebuilds"))
	bool bDisableSpatialRebuilds = true;

	/* Dynamic Spatial Frequency */
	// How many buckets to spread dynamic, spatialized actors across.
	// High number = more buckets = smaller effective replication frequency.
	// This happens before individual actors do their own NetUpdateFrequency check.
	UPROPERTY(EditAnywhere, Category = DynamicSpatialFrequency, meta = (ConsoleVariable = "OW.ReplicationGraph.DynamicActorFrequencyBuckets"))
	int32 DynamicActorFrequencyBuckets = 3;

	// Array of Custom Settings for Specific Classes 
	UPROPERTY(config, EditAnywhere, Category = ReplicationGraph)
	TArray<FRepGraphActorClassSettings> ClassSettings;
};
