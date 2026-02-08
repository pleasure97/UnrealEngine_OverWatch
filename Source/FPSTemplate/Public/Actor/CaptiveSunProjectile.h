// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/OWProjectile.h"
#include "CaptiveSunProjectile.generated.h"

class UGameplayEffect; 

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> LacerationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileVelocity = 3250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 1000.f;

	UFUNCTION()
	void BurstWhenHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;

	virtual void LifeSpanExpired() override;

	FDamageEffectParams MakeDamageEffectParams();
private:
	bool bHasExploded = false;

	void Burst(const FVector& ProjectileLocation, AActor* DirectHitActor = nullptr);
};
