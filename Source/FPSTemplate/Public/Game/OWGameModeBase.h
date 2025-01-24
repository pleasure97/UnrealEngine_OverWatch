// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DS_GameModeBase.h"
#include "OWGameModeBase.generated.h"

class UHeroInfo; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWGameModeBase : public ADS_GameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Hero Information")
	TObjectPtr<UHeroInfo> HeroInfo;
	
};
