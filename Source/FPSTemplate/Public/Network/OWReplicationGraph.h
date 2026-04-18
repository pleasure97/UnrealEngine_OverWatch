// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "OWReplicationGraph.generated.h"

UENUM()
enum class EClassRepNodeMapping : uint8
{
	NotRouted,						// Doesn't map to any node. Used for special case actors that handled by special case nodes
	RelevantAllConnections,			// Routes to an AlwaysRelevantNode or AlwaysRelevantStreamingLevelNode node
	Spatialize_Static,				// Routes to GridNode: these actors don't move and don't need to be updated every frame.
	Spatialize_Dynamic,				// Routes to GridNode: these actors mode frequently and are updated once per frame.
	Spatialize_Dormancy,			// Routes to GridNode: While dormant we treat as static. When flushed/not dormant dynamic. Note this is for things that "move while not dormant".
};

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

	/* Team */
	void AddActorToTeamNode(AActor* Actor, int32 TeamID);
	void RemoveActorFromTeamNode(AActor* Actor, int32 TeamID);

private:
	UPROPERTY()
	TObjectPtr<UReplicationGraphNode_GridSpatialization2D> GridNode;

	UPROPERTY()
	TObjectPtr<UReplicationGraphNode_ActorList> AlwaysRelevantNode;

	TClassMap<EClassRepNodeMapping> ClassRepNodePolicies; 

	void RegisterClassRepNodeMappings(UClass* Class, EClassRepNodeMapping Policy);
};
