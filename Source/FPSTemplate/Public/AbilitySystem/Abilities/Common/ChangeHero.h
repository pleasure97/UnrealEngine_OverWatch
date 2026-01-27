// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "ChangeHero.generated.h"

class UHeroSelectionOverlay;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UChangeHero : public UOWGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UHeroSelectionOverlay> HeroSelectionOverlayClass;
};
