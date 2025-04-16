// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "HealingPylon.generated.h"

class AHealingSunStone; 
class UDemolishPylon; 
class UOWUserWidget; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHealingPylon : public UOWGameplayAbility
{
	GENERATED_BODY()

	friend class UDemolishPylon; 
public:
	/* Healing Sun Stone Spawn */
	UPROPERTY()
	TObjectPtr<AHealingSunStone> HealingSunStone; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf <AHealingSunStone> HealingSunStoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector PylonRelativeLocation = FVector(56.f, 2.f, 56.f); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PylonThrowingSpeed = 800.f; 

	/* UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UOWUserWidget> PylonDestroyedWidgetClass;

	UPROPERTY()
	TObjectPtr<UOWUserWidget> PylonDestroyedWidget;

	FTimerHandle RemoveWidgetTimerHandle;
	FTimerHandle DamageCheckTimerHandle; 

	UFUNCTION()
	void RemoveWidget();

	/* Cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GeneralCooldownTime = 6.f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamagedCooldownTime = 14.f;

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnHealingSunStoneDestroyed(AActor* DestroyedActor); 

	void OnHealingSunStoneDamaged(const FOnAttributeChangeData& Data);

	UFUNCTION()
	void UpdateSunStoneDamagedState(); 

	void ApplyDynamicCooldown(float CooldownTime); 

	bool bSunStoneDamaged = false;
};
