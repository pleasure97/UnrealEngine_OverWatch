// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
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
struct FHeroDamagedInfo
{
	GENERATED_BODY()

	FHeroDamagedInfo() {};

	UPROPERTY()
	FGameplayTag DamageTag = FGameplayTag();

	UPROPERTY()
	TObjectPtr<APlayerState> SourcePlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerState> TargetPlayerState = nullptr;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY()
	double DamageTimeSeconds = 0.f;
};

USTRUCT(BlueprintType)
struct FHeroDebuffedInfo
{
	GENERATED_BODY()

	FHeroDebuffedInfo() {}; 

	UPROPERTY()
	FGameplayTag DebuffTag = FGameplayTag(); 

	UPROPERTY()
	TObjectPtr<APlayerState> SourcePlayerState = nullptr; 

	UPROPERTY()
	TObjectPtr<APlayerState> TargetPlayerState = nullptr; 

	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle = FGameplayEffectContextHandle(); 

	UPROPERTY()
	float DebuffDamage = 0.f; 

	UPROPERTY()
	float DebuffDuration = 0.f; 

	UPROPERTY()
	float DebuffFrequency = 0.f; 

	UPROPERTY()
	double DebuffTimeSeconds = 0.f; 
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

USTRUCT(BlueprintType)
struct FHeroRespawnInfo
{
	GENERATED_BODY()

	FHeroRespawnInfo() {}

	FHeroRespawnInfo(APlayerState* InOwnerPlayerState)
		: OwnerPlayerState(InOwnerPlayerState)
	{
	}

	UPROPERTY()
	TObjectPtr<APlayerState> OwnerPlayerState = nullptr;
};

USTRUCT(BlueprintType)
struct FHeroResetInfo
{
	GENERATED_BODY()

	FHeroResetInfo() {}

	FHeroResetInfo(AActor* InOwnerPlayerState)
		: OwnerPlayerState(InOwnerPlayerState)
	{
	}

	UPROPERTY()
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
};

USTRUCT(BlueprintType)
struct FInteractionDurationInfo
{
	GENERATED_BODY()

	FInteractionDurationInfo() {}

	FInteractionDurationInfo(APlayerState* InPlayerState, float InInteractionDuraction)
		: PlayerState(InPlayerState), InteractionDuration(InInteractionDuraction)
	{

	}

	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState = nullptr; 

	UPROPERTY()
	float InteractionDuration = 0.f; 
};

USTRUCT(BlueprintType)
struct FOWVerbMessage
{
	GENERATED_BODY()

	FOWVerbMessage() {}

	UPROPERTY()
	FGameplayTag Verb = FGameplayTag(); 

	UPROPERTY()
	TObjectPtr<AActor> Instigator = nullptr; 

	UPROPERTY()
	TObjectPtr<AActor> Target = nullptr; 

	UPROPERTY()
	FGameplayTagContainer InstigatorTags = FGameplayTagContainer();

	UPROPERTY()
	FGameplayTagContainer TargetTags = FGameplayTagContainer();

	UPROPERTY()
	FGameplayTagContainer ContextTags = FGameplayTagContainer();

	UPROPERTY()
	float Magnitude = 0.f;
};

UENUM(BlueprintType)
enum EOccupationState : uint8
{
	Start UMETA(DisplayName = "Start"),
	Stop UMETA(DisplayName = "Stop"),
	Contesting UMETA(DisplayName = "Contesting"),
	Complete UMETA(DisplayName = "Complete"), 
	None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FOccupationInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EOccupationState> OccupationState = EOccupationState::None; 

	UPROPERTY()
	int32 NumDefenders = 0; 

	UPROPERTY()
	int32 NumAttackers = 0; 

	UPROPERTY()
	float OccupationProgress = 0.f; 
};
