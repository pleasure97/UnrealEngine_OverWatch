// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "HeroInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect; 

UENUM(BlueprintType)
enum class EHeroName : uint8
{
	None,
	ILLIARI,
	REINHARDT, 
};

UENUM(BlueprintType)
enum class EHeroClass : uint8
{
	Damage, 
	Tank, 
	Support, 
};

USTRUCT(BlueprintType)
struct FOWAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
};

USTRUCT(BlueprintType)
struct FOWHeroInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EHeroClass HeroClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FOWAbilityInfo> Abilities; 
};

UCLASS()
class FPSTEMPLATE_API UHeroInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Hero Defaults")
	TMap<EHeroName, FOWHeroInfo> HeroInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FOWAbilityInfo FindAbilityInfoForTag(const EHeroName& HeroName, const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
