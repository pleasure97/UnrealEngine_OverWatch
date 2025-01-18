// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DefensiveAttributeInfo.generated.h"

class UTexture2D; 

USTRUCT(BlueprintType)
struct FAttributeDefensiveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DefensiveAttributeTag = FGameplayTag(); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor Tint_Fill = FLinearColor(0.f, 0.f, 0.f, 1.f);

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f; 
};

UCLASS()
class FPSTEMPLATE_API UDefensiveAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Information")
	TArray<FAttributeDefensiveInfo> DefensiveAttributeInformation; 

	FAttributeDefensiveInfo FindDefensiveAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const; 	
};
