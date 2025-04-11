// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "DemolishPylon.generated.h"

class AHealingSunStone; 
class UOWUserWidget;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UDemolishPylon : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TWeakObjectPtr<AHealingSunStone> Pylon; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UOWUserWidget> PylonDestroyedWidgetClass; 

	UPROPERTY()
	TObjectPtr<UOWUserWidget> PylonDestroyedWidget; 

	FTimerHandle RemoveWidgetTimerHandle; 

	UFUNCTION()
	void RemoveWidget(); 
protected:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		OUT FGameplayTagContainer* OptionalRelevantTags) const override; 

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override; 
};
