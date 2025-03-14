// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWAbilityTypes.h"
#include "OWProjectile.generated.h"

class UProjectileMovementComponent; 
class USphereComponent; 
class UAudioComponent;
class UNiagaraSystem; 

UCLASS()
class FPSTEMPLATE_API AOWProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AOWProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement; 

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams; 


protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override; 

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(); 

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult); 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere; 

	bool IsValidOverlap(AActor* OtherActor); 

	bool bHit = false;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent; 

private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 1.5f; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound; 
};
