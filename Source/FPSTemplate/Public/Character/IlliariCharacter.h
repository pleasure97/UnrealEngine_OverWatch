// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacter.h"
#include "IlliariCharacter.generated.h"

class AOWGATargetActor_LineTrace;
class UNiagaraComponent; 
class UNiagaraSystem; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AIlliariCharacter : public AOWCharacter
{
	GENERATED_BODY()
	
public:
	AIlliariCharacter(); 

	virtual AOWGATargetActor_LineTrace* GetLineTraceTargetActor_Implementation() override;
	virtual FVector GetProjectileStartLocation_Implementation() const override;
	virtual FTransform GetFirstPersonWeaponSocketTransform_Implementation() override; 
	virtual FTransform GetThirdPersonWeaponSocketTransform_Implementation() override;

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetHealingRayNiagaraComponent() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> HealingRayNiagaraSystem;

	UFUNCTION(BlueprintCallable)
	void RegenerateSolarRifle();

	UFUNCTION(BlueprintCallable)
	void RegenerateHealingRay(); 

	UFUNCTION(BlueprintCallable)
	void SetHealingRayRegenTimerHandle(FTimerHandle InHealingRayRegenTimerHandle); 

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetFirstPersonWeapon() const; 

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetThirdPersonWeapon() const; 

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FirstPersonWeapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ThirdPersonWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> HealingPylon;

	UPROPERTY()
	TObjectPtr<AOWGATargetActor_LineTrace> LineTraceTargetActor;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> HealingRayNiagaraComponent;

	FTimerHandle HealingRayRegenTimerHandle; 
};
