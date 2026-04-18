// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/OWReplicationGraph.h"
#include "Network/OWReplicationGraphSettings.h"

namespace OW::ReplicationGraph
{
	/* Fast Shared Path */
	int32 EnableFastSharedPath = 1;
	static FAutoConsoleVariableRef CVarOWRepEnableFastSharedPath(TEXT("OW.ReplicationGraph.EnableFastSharedPath"), EnableFastSharedPath, TEXT(""), ECVF_Default);

	// How much bandwidth to use for FastShared movement updates. This is counted independently of the NetDriver's target bandwidth.
	int32 TargetKBytesSecFastSharedPath = 10;
	static FAutoConsoleVariableRef CVarOWRepTargetKBytesSecFastSharedPath(TEXT("OW.ReplicationGraph.TargetKBytesSecFastSharedPath"), TargetKBytesSecFastSharedPath, TEXT(""), ECVF_Default);

	float FastSharedPathCullDistPercentage = 0.8f;
	static FAutoConsoleVariableRef CVarOWRepFastSharedPathCullDistPct(TEXT("OW.ReplicationGraph.FastSharedPathCullDistPercentage"), FastSharedPathCullDistPercentage, TEXT(""), ECVF_Default);

	/* Destruction Information */
	float DestructionInfoMaxDistance = 30000.f;
	static FAutoConsoleVariableRef CVarOWRepGraphDestructMaxDistance(TEXT("OW.ReplicationGraph.DestructInfo.MaxDistance"), DestructionInfoMaxDistance, TEXT("Max distance (not squared) to rep destruct infos at"), ECVF_Default);

	/* Spatial Grid */
	float CellSize = 10000.f;
	static FAutoConsoleVariableRef CVarOWRepGraphCellSize(TEXT("OW.ReplicationGraph.CellSize"), CellSize, TEXT(""), ECVF_Default);

	// Essentially "Min X" for replication. This is just an initial value. The system will reset itself if actors appears outside of this.
	float SpatialBiasX = -150000.f;
	static FAutoConsoleVariableRef CVarLyraRepGraphSpatialBiasX(TEXT("OW.ReplicationGraph.SpatialBiasX"), SpatialBiasX, TEXT(""), ECVF_Default);

	// Essentially "Min Y" for replication. This is just an initial value. The system will reset itself if actors appears outside of this.
	float SpatialBiasY = -200000.f;
	static FAutoConsoleVariableRef CVarLyraRepSpatialBiasY(TEXT("OW.ReplicationGraph.SpatialBiasY"), SpatialBiasY, TEXT(""), ECVF_Default);

	int32 EnableSpatialRebuilds = 0;
	static FAutoConsoleVariableRef CVarOWRepEnableSpatialRebuilds(TEXT("OW.ReplicationGraph.EnableSpatialRebuilds"), EnableSpatialRebuilds, TEXT(""), ECVF_Default);

	/* Dynamic Spatial Frequency */
	// How many buckets to spread dynamic, spatialized actors across.
	// High number = more buckets = smaller effective replication frequency.
	// This happens before individual actors do their own NetUpdateFrequency check.
	int32 DynamicActorFrequencyBuckets = 3;
	static FAutoConsoleVariableRef CVarLyraRepDynamicActorFrequencyBuckets(TEXT("OW.ReplicationGraph.DynamicActorFrequencyBuckets"), DynamicActorFrequencyBuckets, TEXT(""), ECVF_Default);

	UReplicationDriver* ConditionalCreateReplicationDriver(UNetDriver* ForNetDriver, UWorld* World)
	{
		// Only create for GameNetDriver
		if (World && ForNetDriver && ForNetDriver->NetDriverName == NAME_GameNetDriver)
		{
			const UOWReplicationGraphSettings* OWReplicationGraphSettings = GetDefault<UOWReplicationGraphSettings>();

			// Enable/Disable via developer settings
			if (OWReplicationGraphSettings && !OWReplicationGraphSettings->bEnableReplicationGraph)
			{
				UE_LOG(LogTemp, Display, TEXT("Replication graph is disabled via OWReplicationGraphSettings."));
				return nullptr;
			}

			UE_LOG(LogTemp, Display, TEXT("Replication graph is enabled for %s in world %s."), *GetNameSafe(ForNetDriver), *GetPathNameSafe(World));

			TSubclassOf<UOWReplicationGraph> GraphClass;
			if (OWReplicationGraphSettings)
			{
				GraphClass = OWReplicationGraphSettings->DefaultReplicationGraphClass.TryLoadClass<UOWReplicationGraph>();
			}

			if (GraphClass.Get() == nullptr)
			{
				GraphClass = UOWReplicationGraph::StaticClass();
			}

			UOWReplicationGraph* OWReplicationGraph = NewObject<UOWReplicationGraph>(GetTransientPackage(), GraphClass.Get());
			return OWReplicationGraph;
		}

		return nullptr;
	}
}

UOWReplicationGraph::UOWReplicationGraph()
{
	if (!UReplicationDriver::CreateReplicationDriverDelegate().IsBound())
	{
		UReplicationDriver::CreateReplicationDriverDelegate().BindLambda(
			[](UNetDriver* ForNetDriver, const FURL& URL, UWorld* World) -> UReplicationDriver*
			{
				return OW::ReplicationGraph::ConditionalCreateReplicationDriver(ForNetDriver, World);
			});
	}
}

void UOWReplicationGraph::InitGlobalActorClassSettings()
{
	Super::InitGlobalActorClassSettings(); 

	// Always Replicate (Game State, Game Mode)
	//RegisterClassRepNodeMappings(AGameStateBase::StaticClass(), EClassRepNodeMapping::RelevantAllConnections);

}

void UOWReplicationGraph::InitGlobalGraphNodes()
{
}

void UOWReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* RepGraphConnection)
{
}

void UOWReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo)
{
}

void UOWReplicationGraph::RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo)
{
}

void UOWReplicationGraph::AddActorToTeamNode(AActor* Actor, int32 TeamID)
{
}

void UOWReplicationGraph::RemoveActorFromTeamNode(AActor* Actor, int32 TeamID)
{
}

void UOWReplicationGraph::RegisterClassRepNodeMappings(UClass* Class, EClassRepNodeMapping Policy)
{
}
