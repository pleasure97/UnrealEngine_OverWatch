// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCueNotify_CaptiveSun.generated.h"

class ACaptiveSunProjectile; 
class AIlliariCharacter;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AGameplayCueNotify_CaptiveSun : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACaptiveSunProjectile> CaptiveSunProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "Captive Sun")
	ACaptiveSunProjectile* SpawnCaptiveSun(AIlliariCharacter* Illiari);
};
