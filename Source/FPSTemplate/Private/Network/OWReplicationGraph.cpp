// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/OWReplicationGraph.h"
#include "Network/OWReplicationGraphSettings.h"
#include "Character/OWCharacter.h"
#include "Engine/ServerStatReplicator.h"
#include "Player/OWPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"

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
	static FAutoConsoleVariableRef CVarOWRepGraphSpatialBiasX(TEXT("OW.ReplicationGraph.SpatialBiasX"), SpatialBiasX, TEXT(""), ECVF_Default);

	// Essentially "Min Y" for replication. This is just an initial value. The system will reset itself if actors appears outside of this.
	float SpatialBiasY = -200000.f;
	static FAutoConsoleVariableRef CVarOWRepSpatialBiasY(TEXT("OW.ReplicationGraph.SpatialBiasY"), SpatialBiasY, TEXT(""), ECVF_Default);

	int32 DisableSpatialRebuilds = 1;
	static FAutoConsoleVariableRef CVarOWRepEnableSpatialRebuilds(TEXT("OW.ReplicationGraph.DisableSpatialRebuilds"), DisableSpatialRebuilds, TEXT(""), ECVF_Default);

	/* Dynamic Spatial Frequency */
	// How many buckets to spread dynamic, spatialized actors across.
	// High number = more buckets = smaller effective replication frequency.
	// This happens before individual actors do their own NetUpdateFrequency check.
	int32 DynamicActorFrequencyBuckets = 3;
	static FAutoConsoleVariableRef CVarOWRepDynamicActorFrequencyBuckets(TEXT("OW.ReplicationGraph.DynamicActorFrequencyBuckets"), DynamicActorFrequencyBuckets, TEXT(""), ECVF_Default);

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

	LazyRegisterNotLoadedClass(); 

	// Get 'Replication Graph Developer Settings' from 'Project Settings'
	const UOWReplicationGraphSettings* OWReplicationGraphSettings = GetDefault<UOWReplicationGraphSettings>();
	check(OWReplicationGraphSettings);
	AddClassReplicationInfoFromSettings(OWReplicationGraphSettings);

	TArray<UClass*> AllReplicatedClasses;
	RegisterReplicatedClass(AllReplicatedClasses);

	RegisterCharacterClassReplicationInfo(); 

	// Setup Actor List Frequency Buckets
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.ListSize = 12; 
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.NumBuckets = OW::ReplicationGraph::DynamicActorFrequencyBuckets;
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.BucketThresholds.Reset();
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.EnableFastPath = (OW::ReplicationGraph::EnableFastSharedPath > 0);
	UReplicationGraphNode_ActorListFrequencyBuckets::DefaultSettings.FastPathFrameModulo = 1;

	RPCSendPolicyMap.Reset(); 
	// Set FClassReplicationInfo based on legacy settings from all replicated classes
	for (UClass* ReplicatedClass : AllReplicatedClasses)
	{
		RegisterClassReplicationInfo(ReplicatedClass);
	}

	// Calculate 'Destruct Info Max Distance Squared'
	DestructInfoMaxDistanceSquared = OW::ReplicationGraph::DestructionInfoMaxDistance * OW::ReplicationGraph::DestructionInfoMaxDistance;

	// Add to RPC_Multicast_OpenChannelForClass map
	RPC_Multicast_OpenChannelForClass.Reset();
	RPC_Multicast_OpenChannelForClass.Set(AActor::StaticClass(), true); // Open channels for multicast RPCs by default
	RPC_Multicast_OpenChannelForClass.Set(AController::StaticClass(), false); // multicasts should never open channels on Controllers since opening a channel on a non-owner breaks the Controller's replication.
	RPC_Multicast_OpenChannelForClass.Set(AServerStatReplicator::StaticClass(), false);

	for (const FRepGraphActorClassSettings& ActorClassSettings : OWReplicationGraphSettings->ClassSettings)
	{
		if (ActorClassSettings.bAddToRPC_Multicast_OpenChannelForClassMap)
		{
			if (UClass* StaticActorClass = ActorClassSettings.GetStaticActorClass())
			{
				RPC_Multicast_OpenChannelForClass.Set(StaticActorClass, ActorClassSettings.bRPC_Multicast_OpenChannelForClass);
			}
		}
	}
}

void UOWReplicationGraph::InitGlobalGraphNodes()
{
	// "Spatial" Actors
	GridNode = CreateNewNode<UReplicationGraphNode_GridSpatialization2D>(); 
	GridNode->CellSize = OW::ReplicationGraph::CellSize;
	GridNode->SpatialBias = FVector2D(OW::ReplicationGraph::SpatialBiasX, OW::ReplicationGraph::SpatialBiasY); 

	if (OW::ReplicationGraph::DisableSpatialRebuilds)
	{
		// Disable All Sptial Rebuilding
		GridNode->AddToClassRebuildDenyList(AActor::StaticClass()); 
	}

	AddGlobalGraphNode(GridNode);

	// "Always Relevant" Actors
	AlwaysRelevantNode = CreateNewNode<UReplicationGraphNode_ActorList>(); 
	AddGlobalGraphNode(AlwaysRelevantNode); 

	// "Player State" 
	UOWReplicationGraphNode_PlayerStateFrequencyLimiter* PlayerStateNode = CreateNewNode<UOWReplicationGraphNode_PlayerStateFrequencyLimiter>(); 
	AddGlobalGraphNode(PlayerStateNode);
}

void UOWReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* RepGraphConnection)
{
	Super::InitConnectionGraphNodes(RepGraphConnection); 

	// Create New Node - Always Relevant Connection
	UOWReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode =
		CreateNewNode< UOWReplicationGraphNode_AlwaysRelevant_ForConnection>(); 

	// This node needs to know when client levels go in and out of visibility
	RepGraphConnection->OnClientVisibleLevelNameAdd.AddUObject(
		AlwaysRelevantConnectionNode, &UOWReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityAdd);
	RepGraphConnection->OnClientVisibleLevelNameRemove.AddUObject(
		AlwaysRelevantConnectionNode, &UOWReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityRemove);

	// Add Always Relevant Connection Node to Rep Graph Connection
	AddConnectionGraphNode(AlwaysRelevantConnectionNode, RepGraphConnection);
}

void UOWReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo)
{
	EClassRepNodeMapping ClassMappingPolicy = GetClassMappingPolicy(ActorInfo.Class);
	switch (ClassMappingPolicy)
	{
	case EClassRepNodeMapping::NotRouted:
	{
		break;
	}
	case EClassRepNodeMapping::RelevantAllConnections:
	{
		// Streaming Level (Name_None) = General
		if (ActorInfo.StreamingLevelName == NAME_None)
		{
			AlwaysRelevantNode->NotifyAddNetworkActor(ActorInfo);
		}
		// If Actor belongs to a specific level, it is stored in the AlwaysRelevantStreamingLevelActors map 
		// and managed to be sent only to clients loaded with that level
		else
		{
			FActorRepListRefView& RepList = AlwaysRelevantStreamingLevelActors.FindOrAdd(ActorInfo.StreamingLevelName);
			RepList.ConditionalAdd(ActorInfo.Actor);
		}
		break;
	}
	case EClassRepNodeMapping::Spatialize_Static:
	{
		GridNode->AddActor_Static(ActorInfo, GlobalInfo);
		break;
	}
	case EClassRepNodeMapping::Spatialize_Dynamic:
	{
		GridNode->AddActor_Dynamic(ActorInfo, GlobalInfo);
		break;
	}
	case EClassRepNodeMapping::Spatialize_Dormancy:
	{
		GridNode->AddActor_Dormancy(ActorInfo, GlobalInfo);
		break;
	}
	}
}

void UOWReplicationGraph::RouteRemoveNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo)
{
	EClassRepNodeMapping ClassMappingPolicy = GetClassMappingPolicy(ActorInfo.Class);
	switch (ClassMappingPolicy)
	{
	case EClassRepNodeMapping::NotRouted:
	{
		break;
	}
	case EClassRepNodeMapping::RelevantAllConnections:
	{
		// Streaming Level (Name_None) = General
		if (ActorInfo.StreamingLevelName == NAME_None)
		{
			AlwaysRelevantNode->NotifyRemoveNetworkActor(ActorInfo);
		}
		// If Actor belongs to a specific level, it is stored in the AlwaysRelevantStreamingLevelActors map 
		// and managed to be sent only to clients loaded with that level
		else
		{
			FActorRepListRefView& RepList = AlwaysRelevantStreamingLevelActors.FindChecked(ActorInfo.StreamingLevelName);
			RepList.RemoveFast(ActorInfo.Actor);
		}

		SetActorDestructionInfoToIgnoreDistanceCulling(ActorInfo.GetActor());

		break;
	}
	case EClassRepNodeMapping::Spatialize_Static:
	{
		GridNode->RemoveActor_Static(ActorInfo);
		break;
	}

	case EClassRepNodeMapping::Spatialize_Dynamic:
	{
		GridNode->RemoveActor_Dynamic(ActorInfo);
		break;
	}
	case EClassRepNodeMapping::Spatialize_Dormancy:
	{
		GridNode->RemoveActor_Dormancy(ActorInfo);
		break;
	}
	}
}

void UOWReplicationGraph::ResetGameWorldState()
{
	Super::ResetGameWorldState(); 

	AlwaysRelevantStreamingLevelActors.Empty(); 

	// Reset Game World States of "Always Relevant Connection" Nodes in Connections
	for (UNetReplicationGraphConnection* ConnectionManager : Connections)
	{
		for (UReplicationGraphNode* ConnectionNode : ConnectionManager->GetConnectionGraphNodes())
		{
			if (UOWReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode 
				= Cast<UOWReplicationGraphNode_AlwaysRelevant_ForConnection>(ConnectionNode))
			{
				AlwaysRelevantConnectionNode->ResetGameWorldState();
			}
		}
	}

	// Reset Game World States of "Always Relevant Connection" Nodes in Pending Connections
	for (UNetReplicationGraphConnection* PendingConnectionManager : PendingConnections)
	{
		for (UReplicationGraphNode* PendingConnectionNode : PendingConnectionManager->GetConnectionGraphNodes())
		{
			if (UOWReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantConnectionNode 
				= Cast<UOWReplicationGraphNode_AlwaysRelevant_ForConnection>(PendingConnectionNode))
			{
				AlwaysRelevantConnectionNode->ResetGameWorldState();
			}
		}
	}
}

void UOWReplicationGraph::LazyRegisterNotLoadedClass()
{
	// Setup our lazy init function for classes that are not currently loaded.
	// [Spawn New Class Actor] -> [No Actor Class in GlobalActorReplicationInfoMap] -> [Call Lambda Function]
	// -> [Register Class Rep Node Mappings] -> [Initialize Class Replication Info]
	GlobalActorReplicationInfoMap.SetInitClassInfoFunc(
		[this](UClass* Class, FClassReplicationInfo& ClassReplicationInfo)
		{
			// This needs to run before RegisterClassReplicationInfo.
			RegisterClassRepNodeMappings(Class);

			const bool bHandled = ConditionalInitClassReplicationInfo(Class, ClassReplicationInfo);

			return bHandled;
		}
	);

	// [No Actor Class in ClassRepNodePolicies] -> [GetClassNodeMapping]
	ClassRepNodePolicies.InitNewElement = [this](UClass* Class, EClassRepNodeMapping& NodeMapping)
		{
			NodeMapping = GetClassNodeMapping(Class);
			return true;
		};
}

void UOWReplicationGraph::AddClassReplicationInfoFromSettings(const UOWReplicationGraphSettings* OWReplicationGraphSettings)
{
	// Set Classes Node Mapping (e.g., Player State, Level Script Actor, Player Controller)
	for (const FRepGraphActorClassSettings& ActorClassSettings : OWReplicationGraphSettings->ClassSettings)
	{
		// Default Value of 'bAddClassRepInfoToMap' is true
		if (ActorClassSettings.bAddClassRepInfoToMap)
		{
			if (UClass* StaticActorClass = ActorClassSettings.GetStaticActorClass())
			{
				AddClassReplicationInfo(StaticActorClass, ActorClassSettings.ClassNodeMapping);
			}
		}
	}
}

void UOWReplicationGraph::RegisterReplicatedClass(OUT TArray<UClass*>& AllReplicatedClasses)
{
	for (TObjectIterator<UClass> Iterator; Iterator; ++Iterator)
	{
		UClass* IteratorClass = *Iterator;
		AActor* ActorCDO = Cast<AActor>(IteratorClass->GetDefaultObject());
		if (!ActorCDO || !ActorCDO->GetIsReplicated())
		{
			continue;
		}

		// Skip Skeleton and Reinstantiated Temporary Blueprint Class
		if (IteratorClass->GetName().StartsWith(TEXT("SKEL_")) || IteratorClass->GetName().StartsWith(TEXT("REINST_")))
		{
			continue;
		}

		AllReplicatedClasses.Add(IteratorClass);

		RegisterClassRepNodeMappings(IteratorClass);
	}
}

void UOWReplicationGraph::RegisterCharacterClassReplicationInfo()
{
	// Clear Explicitly Set Classes
	ExplicitlySetClasses.Reset();

	// Setup Character Class Replication Info (Distance / Starvation Priority Scale, Actor Channel Frame Tiemout, CullDistance)
	FClassReplicationInfo CharacterClassReplicationInfo;
	CharacterClassReplicationInfo.DistancePriorityScale = 1.f;
	CharacterClassReplicationInfo.StarvationPriorityScale = 1.f;
	CharacterClassReplicationInfo.ActorChannelFrameTimeout = 4;
	CharacterClassReplicationInfo.SetCullDistanceSquared(AOWCharacter::StaticClass()->GetDefaultObject<AOWCharacter>()->GetNetCullDistanceSquared());

	GlobalActorReplicationInfoMap.SetClassInfo(ACharacter::StaticClass(), CharacterClassReplicationInfo);
	ExplicitlySetClasses.Add(ACharacter::StaticClass());

	// Setup FastShared replication for pawns. 
	// This is called up to once per frame per pawn to see if it wants to send a FastShared update to all relevant connections.
	CharacterClassReplicationInfo.FastSharedReplicationFunc = [](AActor* Actor)
		{
			bool bSuccess = false;
			if (AOWCharacter* OWCharacter = Cast<AOWCharacter>(Actor))
			{
				bSuccess = OWCharacter->UpdateSharedReplication();
			}
			return bSuccess;
		};
	CharacterClassReplicationInfo.FastSharedReplicationFuncName = FName(TEXT("FastSharedReplication"));

	FastSharedPathConstants.MaxBitsPerFrame = (int32)((float)(OW::ReplicationGraph::TargetKBytesSecFastSharedPath * 1024 * 8) / NetDriver->GetNetServerMaxTickRate());
	FastSharedPathConstants.DistanceRequirementPct = OW::ReplicationGraph::FastSharedPathCullDistPercentage;

	GlobalActorReplicationInfoMap.SetClassInfo(AOWCharacter::StaticClass(), CharacterClassReplicationInfo);
	ExplicitlySetClasses.Add(AOWCharacter::StaticClass());
}

void UOWReplicationGraph::RegisterClassReplicationInfo(UClass* ReplicatedClass)
{
	FClassReplicationInfo ClassInfo;
	if (ConditionalInitClassReplicationInfo(ReplicatedClass, ClassInfo))
	{
		GlobalActorReplicationInfoMap.SetClassInfo(ReplicatedClass, ClassInfo);
	}
}

void UOWReplicationGraph::RegisterClassRepNodeMappings(UClass* Class)
{
	EClassRepNodeMapping Mapping = GetClassNodeMapping(Class);
	ClassRepNodePolicies.Set(Class, Mapping);
}

EClassRepNodeMapping UOWReplicationGraph::GetClassNodeMapping(UClass* Class) const
{
	// Not Routed
	if (!Class)
	{
		return EClassRepNodeMapping::NotRouted;
	}

	// Find Class Rep Node Mapping from Policies
	if (const EClassRepNodeMapping* FoundClassRepNodeMapping = ClassRepNodePolicies.FindWithoutClassRecursion(Class))
	{
		return *FoundClassRepNodeMapping;
	}

	AActor* ActorCDO = Cast<AActor>(Class->GetDefaultObject()); 
	// Not Routed 
	if (!ActorCDO || !ActorCDO->GetIsReplicated())
	{
		return EClassRepNodeMapping::NotRouted;
	}

	auto ShouldSpatialize = [](const AActor* CDO)
		{
			return CDO->GetIsReplicated() && (!CDO->bAlwaysRelevant || CDO->bOnlyRelevantToOwner || CDO->bNetUseOwnerRelevancy); 
		};

	// Only handle this class if it differs from its super. There is no need to put every child class explicitly in the graph class mapping
	UClass* SuperClass = Class->GetSuperClass(); 
	if (AActor* SuperCDO = Cast<AActor>(SuperClass->GetDefaultObject()))
	{
		if (SuperCDO->GetIsReplicated() == ActorCDO->GetIsReplicated() &&
			SuperCDO->bAlwaysRelevant == ActorCDO->bAlwaysRelevant &&
			SuperCDO->bOnlyRelevantToOwner == ActorCDO->bAlwaysRelevant &&
			SuperCDO->bNetUseOwnerRelevancy == ActorCDO->bNetUseOwnerRelevancy)
		{
			return GetClassNodeMapping(SuperClass);
		}
	}

	// [Spatialize] - Replicated, Not Always Relevant, Only Relevant to Owner, and Net Use Owner Relevancy
	if (ShouldSpatialize(ActorCDO))
	{
		return EClassRepNodeMapping::Spatialize_Dynamic;
	}
	// [Relevant All Connections] - Replicated, Always Relevant, Not Only Relevant to Onwer, and Net Use Owner Relevancy
	else if (ActorCDO->bAlwaysRelevant && !ActorCDO->bOnlyRelevantToOwner)
	{
		return EClassRepNodeMapping::RelevantAllConnections;
	}

	return EClassRepNodeMapping::NotRouted;
}

EClassRepNodeMapping UOWReplicationGraph::GetClassMappingPolicy(UClass* Class)
{
	EClassRepNodeMapping* ClassMappingPolicyPointer = ClassRepNodePolicies.Get(Class);
	EClassRepNodeMapping ClassMappingPolicy = ClassMappingPolicyPointer ? *ClassMappingPolicyPointer : EClassRepNodeMapping::NotRouted;
	return ClassMappingPolicy;
}

bool UOWReplicationGraph::ConditionalInitClassReplicationInfo(UClass* ReplicatedClass, FClassReplicationInfo& ClassInfo)
{
	// Find from Explicitly Set Classes
	UClass** FoundExplicitlySetClass = ExplicitlySetClasses.FindByPredicate(
		[&](const UClass* SetClass)
		{
			return ReplicatedClass->IsChildOf(SetClass);
		});
	if (FoundExplicitlySetClass)
	{
		return false;
	}

	bool IsClassSpatialized = IsSpatialized(ClassRepNodePolicies.GetChecked(ReplicatedClass)); 

	// Save Replicated Class CDO's Net Cull Distance and Net Update Frequency to Class Info
	InitClassReplicationInfo(ReplicatedClass, OUT ClassInfo, IsClassSpatialized);

	return true;
}

void UOWReplicationGraph::InitClassReplicationInfo(UClass* ReplicatedClass, OUT FClassReplicationInfo& ClassInfo, bool bSpatialize)
{
	// Get CDO from Replicated Class
	AActor* CDO = ReplicatedClass->GetDefaultObject<AActor>(); 

	// If Replicated Class is spatialized, save the Replicated Class CDO's Net Cull Distance Info to Class Info 
	if (bSpatialize)
	{
		ClassInfo.SetCullDistanceSquared(CDO->GetNetCullDistanceSquared()); 
	}

	// Save the Replicated Class CDO's Net Update Frequency to Class Info 
	ClassInfo.ReplicationPeriodFrame = GetReplicationPeriodFrameForFrequency(CDO->GetNetUpdateFrequency());
}

void UOWReplicationGraph::AddClassReplicationInfo(UClass* ReplicatedClass, EClassRepNodeMapping Mapping)
{
	// Warn that there is 'Spatialize_Dynamic' or 'Spatialize_Dormancy' Node that is set to 'bAlwaysRelevant'
	if (IsSpatialized(Mapping))
	{
		if (ReplicatedClass->GetDefaultObject<AActor>()->bAlwaysRelevant)
		{
			UE_LOG(LogTemp, Warning, 
				TEXT("Replicated Class %s is AlwaysRelevant but is initialized into a spatialized node (%s)"), 
				*ReplicatedClass->GetName(), *StaticEnum<EClassRepNodeMapping>()->GetNameStringByValue((int64)Mapping));
		}
	}

	ClassRepNodePolicies.Set(ReplicatedClass, Mapping);
}

void UOWReplicationGraphNode_AlwaysRelevant_ForConnection::GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params)
{
	UOWReplicationGraph* OWReplicationGraph = CastChecked<UOWReplicationGraph>(GetOuter()); 

	ReplicationActorList.Reset(); 

	// Iterate Connection Gather Actor List Parameters' Viewers
	for (const FNetViewer& CurrentNetViewer : Params.Viewers)
	{
		// Collect Viewer (e.g., Player Controller) and Target (e.g., Player)
		ReplicationActorList.ConditionalAdd(CurrentNetViewer.InViewer);
		ReplicationActorList.ConditionalAdd(CurrentNetViewer.ViewTarget);

		// Cast InViewer to Custom Player Controller
		if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(CurrentNetViewer.InViewer))
		{
			// 50% throttling of PlayerStates
			const bool bReplicatePlayerState = (Params.ConnectionManager.ConnectionOrderNum % 2) == (Params.ReplicationFrameNum % 2); 
			if (bReplicatePlayerState)
			{
				// Always return the player state to the owning player. 
				// Simulated proxy player states are handled by UOWReplicationGraphNode_PlayerStateFrequencyLimiter
				if (APlayerState* PS = OWPlayerController->PlayerState)
				{
					if (!bInitializedPlayerState)
					{
						bInitializedPlayerState = true;
						// For Owner, Set ReplicationPeriodFrame = 1 to Ensure the Owner's Player state is synchronized as Quickly as Possible
						FConnectionReplicationActorInfo& ConnectionActorInfo = Params.ConnectionManager.ActorInfoMap.FindOrAdd(PS);
						ConnectionActorInfo.ReplicationPeriodFrame = 1;
					}

					ReplicationActorList.ConditionalAdd(PS);
				}
			}

			// Update Player Controlled Character's Information
			FCachedAlwaysRelevantActorInfo& LastData = PastRelevantActorMap.FindOrAdd(CurrentNetViewer.Connection);
			if (AOWCharacter* OWCharacter = Cast<AOWCharacter>(OWPlayerController->GetPawn()))
			{
				// Check if the state has changed compared to the previous frame, and update the cache if necessary
				UpdateCachedRelevantActor(Params, OWCharacter, LastData.LastViewer);

				if (OWCharacter != CurrentNetViewer.ViewTarget)
				{
					ReplicationActorList.ConditionalAdd(OWCharacter);
				}
			}

			if (AOWCharacter* ViewTargetCharacter = Cast<AOWCharacter>(CurrentNetViewer.ViewTarget))
			{
				// Check if the state has changed compared to the previous frame, and update the cache if necessary
				UpdateCachedRelevantActor(Params, ViewTargetCharacter, LastData.LastViewTarget);
			}
		}
	}

	// Remove entries for connections that are no longer active
	CleanupCachedRelevantActors(PastRelevantActorMap);

	// Always relevant streaming level actors
	FPerConnectionActorInfoMap& ConnectionActorInfoMap = Params.ConnectionManager.ActorInfoMap;

	TMap<FName, FActorRepListRefView>& AlwaysRelevantStreamingLevelActors = OWReplicationGraph->AlwaysRelevantStreamingLevelActors;

	// Streaming Level Actors
	// Iterate backwards through levels that this client has loaded and requires replication for
	for (int32 Idx = AlwaysRelevantStreamingLevelsNeedingReplication.Num() - 1; Idx >= 0; --Idx)
	{
		// Find Streaming Level 
		const FName& StreamingLevel = AlwaysRelevantStreamingLevelsNeedingReplication[Idx];

		// Ensure the streaming level exists in the global graph settings
		FActorRepListRefView* AlwaysRelevantStreamingLevelActor = AlwaysRelevantStreamingLevelActors.Find(StreamingLevel);
		if (!AlwaysRelevantStreamingLevelActor)
		{
			// No always relevant lists for that level
			AlwaysRelevantStreamingLevelsNeedingReplication.RemoveAtSwap(Idx, EAllowShrinking::No);
			continue;
		}

		FActorRepListRefView& ReplicationList = *AlwaysRelevantStreamingLevelActor;

		if (ReplicationList.Num() > 0)
		{
			// Even if a level is "Always Relevant," we can skip the entire list if ALL actors in it are dormant.
			bool bAllDormant = true;
			// Iterate Replication List 
			for (FActorRepListType Actor : ReplicationList)
			{
				FConnectionReplicationActorInfo& ConnectionActorInfo = ConnectionActorInfoMap.FindOrAdd(Actor);
				if (ConnectionActorInfo.bDormantOnConnection == false)
				{
					// At least one actor is active, we must replicate this list
					bAllDormant = false;
					break;
				}
			}

			if (bAllDormant)
			{
				// All Actors are Dormant - Remove from active list to avoid redundant checks in future frames
				AlwaysRelevantStreamingLevelsNeedingReplication.RemoveAtSwap(Idx, EAllowShrinking::No);
			}
			else
			{
				// Level Contains Active Actors - Add the entire list to the gathering result
				Params.OutGatheredReplicationLists.AddReplicationActorList(ReplicationList);
			}
		}
	}

	// Add the connection-specific relevant list to the output gathered lists
	Params.OutGatheredReplicationLists.AddReplicationActorList(ReplicationActorList);
}

void UOWReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityAdd(FName LevelName, UWorld* StreamingWorld)
{
	AlwaysRelevantStreamingLevelsNeedingReplication.Add(LevelName);
}

void UOWReplicationGraphNode_AlwaysRelevant_ForConnection::OnClientLevelVisibilityRemove(FName LevelName)
{
	AlwaysRelevantStreamingLevelsNeedingReplication.Remove(LevelName);
}

void UOWReplicationGraphNode_AlwaysRelevant_ForConnection::ResetGameWorldState()
{
	ReplicationActorList.Reset();
	AlwaysRelevantStreamingLevelsNeedingReplication.Empty();
}

UOWReplicationGraphNode_PlayerStateFrequencyLimiter::UOWReplicationGraphNode_PlayerStateFrequencyLimiter()
{
	bRequiresPrepareForReplicationCall = true;
}

void UOWReplicationGraphNode_PlayerStateFrequencyLimiter::GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params)
{
	// Round-Robin Distribution
	// Use the current frame number to pick one bucket from the 'ReplicationActorLists'.
	// This ensures that if we have N buckets, each PlayerState is replicated once every N frames.
	const int32 ListIdx = Params.ReplicationFrameNum % ReplicationActorLists.Num();
	Params.OutGatheredReplicationLists.AddReplicationActorList(ReplicationActorLists[ListIdx]);

	// Priority Bypass
	// If any PlayerStates have explicitly requested an immediate update (ForceNetUpdate),
	// they are sent immediately regardless of the round-robin schedule.
	if (ForceNetUpdateReplicationActorList.Num() > 0)
	{
		Params.OutGatheredReplicationLists.AddReplicationActorList(ForceNetUpdateReplicationActorList);
	}
}

void UOWReplicationGraphNode_PlayerStateFrequencyLimiter::PrepareForReplication()
{
	// Reset 'Replication Actor Lists' and 'ForceNetUpdateReplicationActorList' to Clear Out-Of-Data from Previous Frame
	ReplicationActorLists.Reset();
	ForceNetUpdateReplicationActorList.Reset();

	// Initialize the First Bucket 
	ReplicationActorLists.AddDefaulted();
	FActorRepListRefView* CurrentList = &ReplicationActorLists[0];

	// Dynamic List Building 
	// We rebuild our lists of player states each frame. 
	// This is not as efficient as it could be but its the simplest way to handle players disconnecting and keeping the lists compact. 
	// If the lists were persistent we would need to defrag them as players left.
	for (TActorIterator<APlayerState> PlayerStateIterator(GetWorld()); PlayerStateIterator; ++PlayerStateIterator)
	{
		APlayerState* PS = *PlayerStateIterator;

		// Ensure the PlayerState is valid for networking (e.g., not pending kill)
		if (!IsActorValidForReplicationGather(PS))
		{
			continue;
		}

		// Load Balancing (Bucketing)
		// If the current list reaches the 'TargetActorsPerFrame' limit, 
		// create a new bucket to distribute the replication load across multiple frames.
		if (CurrentList->Num() >= TargetActorsPerFrame)
		{
			ReplicationActorLists.AddDefaulted();
			CurrentList = &ReplicationActorLists.Last();
		}

		CurrentList->Add(PS);
	}
}
