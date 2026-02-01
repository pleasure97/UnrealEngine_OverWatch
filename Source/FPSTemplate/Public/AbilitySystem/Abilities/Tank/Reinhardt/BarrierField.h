// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "BarrierField.generated.h"

class AEnergyBarrier; 
class UOWUserWidget; 
class ACharacter; 
class UOWAnimInstance; 
class UAnimationMontage; 
class UReinhardtBarrierWidget; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UBarrierField : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	/* Barrier Actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnergyBarrier> EnergyBarrierClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AEnergyBarrier> EnergyBarrier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BarrierSocketName;

	/* Barrier Health Regeneration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> BarrierHealthRegnerateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BarrierHealthRegenTimeAfterAbilityEnds = 2.f;

	/* Character Movement */
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter = nullptr; 

	UPROPERTY(Editanywhere, BlueprintReadOnly)
	float MovementDecreasedPercentage = 0.3f; 

	/* Animation Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BarrierFieldMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BarrierFieldReturnMontage;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
