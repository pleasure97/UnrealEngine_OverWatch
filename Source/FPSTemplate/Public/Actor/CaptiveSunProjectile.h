// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/OWProjectile.h"
#include "CaptiveSunProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API ACaptiveSunProjectile : public AOWProjectile
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Shoot(); 

protected:
	UFUNCTION()
	void Burst(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY()
	float ProjectileVelocity = 3000.f; 

	UPROPERTY()
	float Radius = 800.f;
};
