// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWAbilityTypes.h"
#include "EarthShatterShockwave.generated.h"

UCLASS()
class FPSTEMPLATE_API AEarthShatterShockwave : public AActor
{
	GENERATED_BODY()
	
public:	
	AEarthShatterShockwave();

	void InitializeShockwave(
		float InMaxDistance, float InExpansionSpeed, float InArcAngle, float InVerticalHeight, 
		float InShockwaveDirectHitDistance, float InShockwaveDirectHitDamage, float InShockwaveBaseDamage, 
		FDamageEffectParams InDamageEffectParams);

protected:
	virtual void Tick(float DeltaTime) override;

	void HandleShockwave(float DeltaTime, bool bInDebug);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EarthShatterShockwave|Debug")
	bool bDebug = false;

	UPROPERTY()
	float CurrentRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistance = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExpansionSpeed = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArcAngle = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VerticalHeight = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShockwaveDirectHitDistance = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShockwaveDirectHitDamage = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShockwaveBaseDamage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KnockDownTime = 3.f;

	UPROPERTY()
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TArray<AActor*> HitActors; 
};
