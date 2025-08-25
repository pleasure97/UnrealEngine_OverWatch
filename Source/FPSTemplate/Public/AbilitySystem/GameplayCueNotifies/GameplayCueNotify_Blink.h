// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCueNotify_Blink.generated.h"

class UNiagaraSystem; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AGameplayCueNotify_Blink : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> BlinkEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachPointName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> GhostMaterial;
};
