// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "CaptiveSun.generated.h"

class ACaptiveSunProjectile; 
class UUltimateDuration; 
class AIlliariCharacter;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UCaptiveSun : public UOWDamageGameplayAbility
{
	GENERATED_BODY()

public:
	/* Projectile */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACaptiveSunProjectile> CaptiveSunProjectileClass;

	/* Ultimate Duration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SoaringDuration = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShootDuration = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FlyingDuration = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUltimateDuration> UltimateDurationClass;

protected:
	UFUNCTION(BlueprintCallable, Category = "Captive Sun")
	ACaptiveSunProjectile* SpawnCaptiveSun(AIlliariCharacter* Illiari);
};
