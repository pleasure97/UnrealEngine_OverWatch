// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Message/OWMessageTypes.h"
#include "KillFeedEntry.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UKillFeedEntry : public UObject
{
	GENERATED_BODY()
	
public:
	UKillFeedEntry() {}; 

	inline void Initialize(const FHeroKilledInfo& InMessage)
	{
		Message = InMessage;
	}

	UPROPERTY()
	FHeroKilledInfo Message; 
};
