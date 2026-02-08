// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "HeroDebuffInfo.generated.h"

USTRUCT(BlueprintType)
struct FHeroDebuffUIInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DebuffInstigatorUIText; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DebuffVictimUIText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor DebuffInstigatorUIColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor DebuffVictimUIColor;
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroDebuffInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FHeroDebuffUIInfo* GetHeroDebuffUIInfo(const FGameplayTag& DebuffTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FHeroDebuffUIInfo> HeroDebuffInfoMap;
};
