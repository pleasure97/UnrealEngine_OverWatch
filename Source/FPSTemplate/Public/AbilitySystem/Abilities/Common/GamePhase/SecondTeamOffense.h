// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "SecondTeamOffense.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API USecondTeamOffense : public UOWGamePhaseAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SecondTeamOffenseTime = 240.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CountdownDuration = 10;
};
