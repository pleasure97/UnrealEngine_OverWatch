// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "TracerBlink.generated.h"

class UTracerBlinkWidget; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTracerBlink : public UOWGameplayAbility
{
	GENERATED_BODY()
	
public:
	/* Game Logic */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BlinkDistance = 750.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CastingTime = 0.1f;

	/* Widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTracerBlinkWidget> TracerBlinkWidgetClass;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
