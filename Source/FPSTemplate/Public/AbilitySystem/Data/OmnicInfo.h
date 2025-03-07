// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "OmnicInfo.generated.h"

class UGameplayEffect; 
class UGameplayAbility; 

UENUM(BlueprintType)
enum class EOmnicClass : uint8
{
	Training, 
	Sentry, 
	Tank 
};

USTRUCT(BlueprintType)
struct FOmnicClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes; 

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities; 

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOmnicInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Omnic Class Defaults")
	TMap<EOmnicClass, FOmnicClassDefaultInfo> OmnicClassInformation; 

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities; 

	FOmnicClassDefaultInfo GetOmnicDefaultInfo(EOmnicClass OmnicClass); 
};
