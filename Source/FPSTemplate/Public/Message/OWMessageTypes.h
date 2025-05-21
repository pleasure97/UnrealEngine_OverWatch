// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWMessageTypes.generated.h"

class UHealthPlateManagerComponent; 
class APlayerState; 

USTRUCT(BlueprintType)
struct FHealthPlateInfo
{
	GENERATED_BODY()

	FHealthPlateInfo() 
		: Pawn(nullptr)
	{} 
	
	FHealthPlateInfo(APawn* InPawn)
		: Pawn(InPawn)
	{}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APawn> Pawn = nullptr; 
};

USTRUCT(BlueprintType)
struct FHealthPlateManagerInfo
{
    GENERATED_BODY()

    FHealthPlateManagerInfo()
        : HealthPlateManagerComponent(nullptr)
    {
    }

    FHealthPlateManagerInfo(UHealthPlateManagerComponent* InHealthPlateManagerComponent)
        : HealthPlateManagerComponent(InHealthPlateManagerComponent)
    {
    }

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UHealthPlateManagerComponent> HealthPlateManagerComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FHeroKilledInfo
{
	GENERATED_BODY()

	FHeroKilledInfo() {}

	FHeroKilledInfo(APlayerState* InSourcePlayerState, APlayerState* InTargetPlayerState)
		: SourcePlayerState(InSourcePlayerState), TargetPlayerState(InTargetPlayerState)
	{
	}

	UPROPERTY()
	TObjectPtr<APlayerState> SourcePlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerState> TargetPlayerState = nullptr;
};
