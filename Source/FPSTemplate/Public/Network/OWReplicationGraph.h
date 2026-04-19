// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "OWReplicationGraphTypes.h"
#include "OWReplicationGraph.generated.h"

class UOWReplicationGraphSettings;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()
	
public:
	UOWReplicationGraph(); 

	/* Replication Graph Overrides */
	virtual void InitGlobalActorClassSettings() override;
	virtual void InitGlobalGraphNodes() override;
	virtual void InitConnectionGraphNodes(UNetReplicationGraphConnection* RepGraphConnection) override;
	virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo) override;
	virtual void RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo) override; 
	virtual void ResetGameWorldState() override;

	/* Spatialization Node */
	UPROPERTY()
	TObjectPtr<UReplicationGraphNode_GridSpatialization2D> GridNode;

	/* Always Relevant Node */
	UPROPERTY()
	TObjectPtr<UReplicationGraphNode_ActorList> AlwaysRelevantNode;

	TMap<FName, FActorRepListRefView> AlwaysRelevantStreamingLevelActors;

private:
	TClassMap<EClassRepNodeMapping> ClassRepNodePolicies; 

	/** Classes that had their replication settings explictly set by code in ULyraReplicationGraph::InitGlobalActorClassSettings */
	TArray<UClass*> ExplicitlySetClasses;

	/* Class Registration */
	void LazyRegisterNotLoadedClass();
	void AddClassReplicationInfoFromSettings(const UOWReplicationGraphSettings* OWReplicationGraphSettings);
	void RegisterReplicatedClass(OUT TArray<UClass*>& AllReplicatedClasses);
	void RegisterCharacterClassReplicationInfo();
	void RegisterClassReplicationInfo(UClass* ReplicatedClass);

	/* Class Rep Node Mapping */
	void RegisterClassRepNodeMappings(UClass* Class);
	EClassRepNodeMapping GetClassNodeMapping(UClass* Class) const;
	EClassRepNodeMapping GetClassMappingPolicy(UClass* Class);

	/* Class Replication Info */
	bool ConditionalInitClassReplicationInfo(UClass* ReplicatedClass, FClassReplicationInfo& ClassInfo);
	void InitClassReplicationInfo(UClass* ReplicatedClass, OUT FClassReplicationInfo& ClassInfo, bool bSpatialize);
	void AddClassReplicationInfo(UClass* ReplicatedClass, EClassRepNodeMapping Mapping); 

	/* Spatialize */
	// Check if Mapping is 'Spatialize_Dynamic' or 'Spatialize_Dormancy'
	bool IsSpatialized(EClassRepNodeMapping InMapping) const { return InMapping >= EClassRepNodeMapping::Spatialize_Static; }
};

UCLASS()
class UOWReplicationGraphNode_AlwaysRelevant_ForConnection : public UReplicationGraphNode_AlwaysRelevant_ForConnection
{
	GENERATED_BODY()

public:
	/* UReplicationGraphNode - Just Override */
	// Called when a network actor is spawned or an actor changes replication status
	virtual void NotifyAddNetworkActor(const FNewReplicatedActorInfo& Actor) override {}
	// Called when a networked actor is being destroyed or no longer wants to replicate
	virtual bool NotifyRemoveNetworkActor(const FNewReplicatedActorInfo& ActorInfo, bool bWarnIfNotFound = true) override { return false; }
	// Called when world changes or when all subclasses should dump any persistent data/lists about replicated actors here. (The new/next world will be set before this is called)
	virtual void NotifyResetAllNetworkActors() override {}

	/* UReplicationGraphNode - Implement */
	virtual void GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params) override;

	void OnClientLevelVisibilityAdd(FName LevelName, UWorld* StreamingWorld);
	void OnClientLevelVisibilityRemove(FName LevelName);

	void ResetGameWorldState();

private:
	TArray<FName, TInlineAllocator<64>> AlwaysRelevantStreamingLevelsNeedingReplication;

	bool bInitializedPlayerState = false;
};

/**
	This is a specialized node for handling PlayerState replication in a frequency limited fashion. 
	It tracks all player states but only returns a subset of them to the replication driver each frame.
	This is an optimization for large player connection counts, and not a requirement.
*/
UCLASS()
class UOWReplicationGraphNode_PlayerStateFrequencyLimiter : public UReplicationGraphNode
{
	GENERATED_BODY()

public:
	UOWReplicationGraphNode_PlayerStateFrequencyLimiter();

	/* UReplicationGraphNode - Just Override */
	virtual void NotifyAddNetworkActor(const FNewReplicatedActorInfo& Actor) override {}
	virtual bool NotifyRemoveNetworkActor(const FNewReplicatedActorInfo& ActorInfo, bool bWarnIfNotFound = true) override { return false; }
	virtual bool NotifyActorRenamed(const FRenamedReplicatedActorInfo& Actor, bool bWarnIfNotFound = true) override { return false; }

	/* UReplicationGraphNode - Implement */
	virtual void GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params) override;
	virtual void PrepareForReplication() override;

	// How many actors we want to return to the replication driver per frame. Will not suppress ForceNetUpdate.
	int32 TargetActorsPerFrame = 2;

private:
	TArray<FActorRepListRefView> ReplicationActorLists;
	FActorRepListRefView ForceNetUpdateReplicationActorList;
};