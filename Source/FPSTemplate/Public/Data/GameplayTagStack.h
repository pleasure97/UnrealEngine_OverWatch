// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagStack.generated.h"

/**
 * 
 */
USTRUCT()
struct FPSTEMPLATE_API FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FGameplayTagStack() {}

	FGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag (InTag), StackCount(InStackCount)
	{

	}

private:
	friend struct FGameplayTagStackContainer; 

	UPROPERTY()
	FGameplayTag Tag; 

	UPROPERTY()
	int32 StackCount = 0; 
};

USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FGameplayTagStackContainer() {}

public:
	void AddStack(FGameplayTag Tag, int32 StackCount); 

	void RemoveStack(FGameplayTag Tag, int32 StackCount); 

	int32 GetStackCount(FGameplayTag Tag) const;

	bool ContainsTag(FGameplayTag Tag) const; 

	/* FFastArraySerializer */
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize); 
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize); 
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize); 
	/* FFastArraySerializer End */

	// Implement NetDeltaSerialize() to use Fast Array Serializer
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms); 
private:
	// Replicated List of Gameplay Tag Stacks 
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks; 

	// Accelerated List of Gameplay Tag Stacks for Queries
	TMap<FGameplayTag, int32> TagToCountMap; 
};

// Notify Reflection System that the FGameplayTagStackContainer provides "Net Delta Serializer" functionality.
template<>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true, 
	};
};