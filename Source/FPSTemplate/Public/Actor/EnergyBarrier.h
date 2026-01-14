// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/AbilityActor.h"
#include "EnergyBarrier.generated.h"

class UBoxComponent; 
class UStaticMeshComponent; 
class UAttributeSet; 
class UGameplayEffect; 


UCLASS()
class FPSTEMPLATE_API AEnergyBarrier : public AAbilityActor
{
	GENERATED_BODY()
	
public:	
	AEnergyBarrier();

	/* Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> BarrierField; 
	
protected:
	virtual void InitAbilityActorInfo() override;

private:
	UFUNCTION()
	void HandleHealth(float NewHealthValue);

	bool bBarrierDangerous = false; 
	bool bBarrierVeryDangerous = false; 
	bool bBarrierDestroyed = false;
};
