// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/GamePhase/OWGamePhaseAbility.h"
#include "SwitchInning.generated.h"

class URoundEnd; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API USwitchInning : public UOWGamePhaseAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SwitchInningTime = 10.f;
};
