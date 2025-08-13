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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHealingSunStone> HealingSunStoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UOWUserWidget> PylonDestroyedWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToRemoveWidget = 3.f; 
};
