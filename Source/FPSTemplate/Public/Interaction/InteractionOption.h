// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/InteractInterface.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "InteractionOption.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;
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