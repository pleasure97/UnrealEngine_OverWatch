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
	{
	}

	FHealthPlateInfo(APawn* InPawn)
		: Pawn(InPawn)
	{
	}

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

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> SourcePlayerState = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> TargetPlayerState = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	double DamageTimeSeconds = 0.f;
};

USTRUCT(BlueprintType)
struct FHeroDebuffedInfo
{
	GENERATED_BODY()

	FHeroDebuffedInfo() {}; 

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DebuffTag = FGameplayTag(); 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> SourcePlayerState = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> TargetPlayerState = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectContextHandle EffectContextHandle = FGameplayEffectContextHandle(); 

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	double DebuffTimeSeconds = 0.f; 
};

USTRUCT(BlueprintType)
struct FHeroKilledInfo
{
	GENERATED_BODY()

	FHeroKilledInfo() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> InstigatorPlayerState = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> TargetPlayerState = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer InstigatorTags; 
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

	UPROPERTY(BlueprintReadWrite)
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

	UPROPERTY(BlueprintReadWrite)
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

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	float InteractionDuration = 0.f; 
};

USTRUCT(BlueprintType)
struct FOWVerbMessage
{
	GENERATED_BODY()

	FOWVerbMessage() {}

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Verb = FGameplayTag(); 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Target = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer InstigatorTags = FGameplayTagContainer();

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer TargetTags = FGameplayTagContainer();

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer ContextTags = FGameplayTagContainer();

	UPROPERTY(BlueprintReadWrite)
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

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EOccupationState> OccupationState = EOccupationState::None; 

	UPROPERTY(BlueprintReadWrite)
	float OccupationProgress = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	int32 OccupationTeamID = -1; 
};


USTRUCT(BlueprintType)
struct FRecallState
{
	GENERATED_BODY()

public:
	FRecallState()
		: TimeSeconds(0.f), Location(FVector::ZeroVector) 
	{}

	FRecallState(float InTimeSeconds, FVector InLocation)
		: TimeSeconds(InTimeSeconds), Location(InLocation)
	{}

	UPROPERTY(BlueprintReadWrite)
	float TimeSeconds = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
};