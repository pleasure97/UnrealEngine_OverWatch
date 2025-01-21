// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "OWInputConfig.generated.h"

class UInputAction; 

USTRUCT(BlueprintType)
struct FOWInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr; 

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag(); 
};

UCLASS()
class FPSTEMPLATE_API UOWInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindAbilityInputForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const; 

	UPROPERTY(EditDefaultsOnly)
	TArray<FOWInputAction> AbilityInputActions; 
};
