// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/GameplayTagStack.h"

void FGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	// Check Gameplay Tag is Valid 
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("An Invalid Tag was passed to Gameplay Tag Stack Container."));
		return;
	}

	if (StackCount > 0)
	{
		// Iterate Replicated List of Gameplay Tag Stacks 
		for (FGameplayTagStack& Stack : Stacks)
		{
			// Find GameplayTagStack which has Same GameplayTag from Gameplay Tag Stacks 
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount; 
				Stack.StackCount = NewCount; 
				TagToCountMap[Tag] = NewCount; 
				MarkItemDirty(Stack); 
				return; 
			}
		}

		FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount); 
		MarkItemDirty(NewStack); 
		TagToCountMap.Add(Tag, StackCount); 
	}
}

void FGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	// Check Gameplay Tag is Valid 
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("An Invalid Tag was passed to Gameplay Tag Stack Container."));
		return;
	}

	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FGameplayTagStack& Stack = *It; 
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{
					It.RemoveCurrent(); 
					TagToCountMap.Remove(Tag); 
					MarkArrayDirty(); 
				}
				else
				{
					const int32 NewCount = Stack.StackCount - StackCount; 
					Stack.StackCount = NewCount; 
					TagToCountMap[Tag] = NewCount; 
					MarkItemDirty(Stack); 
				}
				return; 
			}
		}
	}
}

int32 FGameplayTagStackContainer::GetStackCount(FGameplayTag Tag) const
{
	return TagToCountMap.FindRef(Tag); 
}

bool FGameplayTagStackContainer::ContainsTag(FGameplayTag Tag) const
{
	return TagToCountMap.Contains(Tag); 
}

/* FFastArraySerializer */
void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 RemovedIndex : RemovedIndices)
	{
		const FGameplayTag Tag = Stacks[RemovedIndex].Tag; 
		TagToCountMap.Remove(Tag); 
	}
}

void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 AddedIndex : AddedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[AddedIndex]; 
		TagToCountMap.Add(Stack.Tag, Stack.StackCount); 
	}
}

void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 ChangedIndex : ChangedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[ChangedIndex]; 
		TagToCountMap[Stack.Tag] = Stack.StackCount; 
	}
}
/* FFastArraySerializer End */

bool FGameplayTagStackContainer::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this); 
}


