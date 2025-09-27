// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "InteractionOption.generated.h"

class IInteractInterface;
class AController; 

USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteractInterface> InteractableActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionText;

	/* Methods of Interaction */
	// 1. Place Interaction Ability on the Avatar that they can Activate when they Perform Interaction 
	// 
	// The Ability to Grant the Avatar when they Get Near Interactable Objects 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;

	// 2. Allow Object We're Interacting with to Have its Own Ability System and Interaction Ability 
	//
	// Ability System Component of Interactable Actor that Can be Used for Interactable Ability Handle and Sending Event 
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> InteractableActorASC = nullptr; 

	// Ability Spec to Activate on the Object for this Option 
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle InteractableActorAbilityHandle; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;

	/* Custom Comparison Operator */
	FORCEINLINE bool operator==(const FInteractionOption& Other) const
	{
		return InteractableActor == Other.InteractableActor &&
			InteractionAbilityToGrant == Other.InteractionAbilityToGrant &&
			InteractableActorASC == Other.InteractableActorASC &&
			InteractableActorAbilityHandle == Other.InteractableActorAbilityHandle &&
			InteractionWidgetClass == Other.InteractionWidgetClass &&
			InteractionText.IdenticalTo(Other.InteractionText) ;
	}

	FORCEINLINE bool operator!=(const FInteractionOption& Other) const
	{
		return !operator==(Other);
	}

	FORCEINLINE bool operator<(const FInteractionOption& Other) const
	{
		return InteractableActor.GetInterface() < Other.InteractableActor.GetInterface(); 
	}
};

USTRUCT(BlueprintType)
struct FInteractionQuery
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> RequestingAvatarActor;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AController> RequestingController;
};