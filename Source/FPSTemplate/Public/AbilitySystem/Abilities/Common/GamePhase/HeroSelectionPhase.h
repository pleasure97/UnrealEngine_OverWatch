// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "GameplayTagContainer.h"
#include "HeroSelectionPhase.generated.h"

class UHeroSelectionOverlay; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroSelectionPhase : public UOWGamePhaseAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HeroSelectionTime = 30.f; 
};
