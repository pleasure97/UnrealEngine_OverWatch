// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "DemolishPylon.generated.h"

class AHealingSunStone; 
class UOWUserWidget;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UDemolishPylon : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHealingSunStone> HealingSunStoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWUserWidget> PylonDestroyedWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownWhenNotDamaged = 14.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownWhenDamaged = 14.f;
};
