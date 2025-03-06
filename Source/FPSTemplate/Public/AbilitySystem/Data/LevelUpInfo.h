// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FOWLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0; 

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 0; 

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 0; 
};


/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FOWLevelUpInfo> LevelUpInformation; 

	int32 FindLevelForXP(int32 XP) const; 
};
