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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWProjectile", meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams; 

	UNiagaraSystem* GetImpactEffect() const;

	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovement() const;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override; 

	bool IsValidOverlap(AActor* OtherActor, bool bHeal);

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(AActor* TargetActor); 

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult); 

	UFUNCTION()
	virtual void OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere; 

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;

	bool bFirstHit = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OWProjectile")
	bool bPenetrable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OWProjectile")
	bool bHealProjectile = false; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWProjectile")
	bool bShouldAttach = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWProjectile")
	float ProjectileLifeSpan = 1.5f; 

	/* Projectile Movement */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/* Impact */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OWProjectile")
	TObjectPtr<UNiagaraSystem> ImpactEffect; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound; 

	/* Looping */
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound; 
};
