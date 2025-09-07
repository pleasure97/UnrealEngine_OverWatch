// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCueNotify_HealingRay.generated.h"

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AGameplayCueNotify_HealingRay : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Niagara")
	TObjectPtr<UNiagaraSystem> HealingRay; 
};
