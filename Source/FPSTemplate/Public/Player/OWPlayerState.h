// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DS_MatchPlayerState.h"
#include "AbilitySystemInterface.h"
#include "OWPlayerState.generated.h"

class UAbilitySystemComponent; 
class UAttributeSet; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWPlayerState : public ADS_MatchPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOWPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet;  }

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 
};
