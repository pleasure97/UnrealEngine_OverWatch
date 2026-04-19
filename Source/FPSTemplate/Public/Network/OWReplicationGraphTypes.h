// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReplicationGraphTypes.h"
#include "OWReplicationGraphTypes.generated.h"

UENUM()
enum class EClassRepNodeMapping : uint8
{
	NotRouted,						// Doesn't map to any node. Used for special case actors that handled by special case nodes
	RelevantAllConnections,			// Routes to an AlwaysRelevantNode or AlwaysRelevantStreamingLevelNode node
	Spatialize_Static,				// Routes to GridNode: these actors don't move and don't need to be updated every frame.
	Spatialize_Dynamic,				// Routes to GridNode: these actors mode frequently and are updated once per frame.
	Spatialize_Dormancy,			// Routes to GridNode: While dormant we treat as static. When flushed/not dormant dynamic. Note this is for things that "move while not dormant".
};

// Actor Class Settings that can be assigned directly to a Class. 
USTRUCT()
struct FRepGraphActorClassSettings
{
	GENERATED_BODY()

	FRepGraphActorClassSettings() = default; 

	// Name of the Class the Settings will be Applied to 
	UPROPERTY(EditAnywhere)
	FSoftClassPath ActorClass;

	// If we should add this Class' RepInfo to the ClassRepNodePolicies Map
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle))
	bool bAddClassRepInfoToMap = true;

	// What ClassNodeMapping we should use when adding Class to ClassRepNodePolicies Map
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bAddClassRepInfoToMap"))
	EClassRepNodeMapping ClassNodeMapping = EClassRepNodeMapping::NotRouted;

	// Should we add this to the RPC_Multicast_OpenChannelForClass map
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle))
	bool bAddToRPC_Multicast_OpenChannelForClassMap = false;

	// If this is added to RPC_Multicast_OpenChannelForClass map then should we actually open a channel or not
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bAddToRPC_Multicast_OpenChannelForClassMap"))
	bool bRPC_Multicast_OpenChannelForClass = true;

	// Retrieve or Load UClass Pointer in Actual Memory from Class Information
	UClass* GetStaticActorClass() const
	{
		// Initialize Static Actor Class
		UClass* StaticActorClass = nullptr;
		const FString ActorClassNameString = ActorClass.ToString();

		// Find	Class with type "/Script/ModuleName.ClassName"
		if (FPackageName::IsScriptPackage(ActorClassNameString))
		{
			StaticActorClass = FindObject<UClass>(nullptr, *ActorClassNameString, EFindObjectFlags::ExactClass);
		}
		// Find Class with type "/Game/Blueprints/MyActor.MyActor_C"
		else
		{
			// Allow blueprints to be used for custom class settings
			StaticActorClass = (UClass*)StaticLoadObject(UClass::StaticClass(), nullptr, *ActorClassNameString);
		}

		return StaticActorClass;
	}
};