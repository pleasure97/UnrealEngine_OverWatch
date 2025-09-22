// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interaction/InteractionOption.h"
#include "InteractInterface.generated.h"

// Collect Interaction Options for Specific Interactable Actor
// Guarantees Each Collected Option Always Carries Correct Interactable Actor
// Centralizes Common Policies in One Place
struct FInteractionOptionCollector
{
public:
	FInteractionOptionCollector(TScriptInterface<IInteractInterface> InContextTarget, TArray<FInteractionOption>& InInteractionOptions)
		: ContextTarget(InContextTarget), InteractionOptions(InInteractionOptions)
	{

	}

	void Add(const FInteractionOption& Option)
	{
		// Safely Append Interaction Option 
		FInteractionOption& InteractionOptionEntry = InteractionOptions.Add_GetRef(Option); 
		InteractionOptionEntry.InteractableActor = ContextTarget; 
	}

private:
	TScriptInterface<IInteractInterface> ContextTarget; 

	TArray<FInteractionOption>& InteractionOptions; 
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSTEMPLATE_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddInteractionOptions(const FInteractionQuery& InteractionQuery, FInteractionOptionCollector& InteractionOptionCollector) = 0; 
};