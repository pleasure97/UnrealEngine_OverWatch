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
	None       UMETA(DisplayName = "None"),
	ILLIARI    UMETA(DisplayName = "Illiari"),
	REINHARDT  UMETA(DisplayName = "Reinhardt"),
	TRACER	   UMETA(DisplayName = "Tracer")
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
	FGameplayTagContainer BlockAbilitiesWithTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer DebuffTags; 

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
	FText HeroDisplayName; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EHeroClass HeroClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> HeroPortrait2D = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> HeroPortrait3D = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APawn> HeroPawnClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FOWAbilityInfo> Abilities; 
};

USTRUCT(BlueprintType)
struct FOWCommonClassInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> ClassIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributes; 
};

UCLASS()
class FPSTEMPLATE_API UHeroInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Hero Defaults")
	TMap<EHeroName, FOWHeroInfo> HeroInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TMap<EHeroClass, FOWCommonClassInfo> CommonClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayEffect>> CommonAttributes; 

	UFUNCTION(BlueprintCallable, Category = "Common Class Defaults")
	TArray<EHeroClass> GetAllHeroClasses() const; 

	FOWAbilityInfo FindAbilityInfoForTag(const EHeroName& HeroName, const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
