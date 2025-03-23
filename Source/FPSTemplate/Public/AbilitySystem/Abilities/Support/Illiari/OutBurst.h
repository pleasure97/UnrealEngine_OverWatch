// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWDamageGameplayAbility.h"
#include "OutBurst.generated.h"

class UAnimMontage; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOutBurst : public UOWDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnInputRelease(float TimeHeld); 

	void Soar(float TimeHeld); 
	
	void TraceEnemies(TArray<AActor*>& TargetEnemies); 

	void DamageAndKnockback(AActor* TargetActor); 

	void PrepareToEndAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> OutburstMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHeldTime = 0.2f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxOutburstVelocity = 1840.f; 

};
