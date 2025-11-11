// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/API/InteractiveAPIData.h"
#include "Game/InteractiveAgentSubsystem.h"
#include "InteractiveAgentManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UInteractiveAgentManager : public UObject
{
	GENERATED_BODY()

public:
	UInteractiveAgentSubsystem* GetInteractiveAgentSubsystem() const; 

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractiveAPIData> InteractiveAPIData;
};
