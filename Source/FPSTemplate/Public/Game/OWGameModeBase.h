// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_MatchGameMode.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/Data/OmnicInfo.h"
#include "OWGameModeBase.generated.h"

class AOWCharacter; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWGameModeBase : public ADS_MatchGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TObjectPtr<UHeroInfo> HeroInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TMap<EHeroName, TSubclassOf<AOWCharacter>> HeroMap; 

	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TObjectPtr<UOmnicInfo> OmnicInfo;

	virtual void BeginPlay() override; 

	virtual void ChangeHero(APlayerController* PlayerController, EHeroName NewHeroName); 
};
