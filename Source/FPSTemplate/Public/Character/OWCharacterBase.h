// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "OWCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(ABSTRACT)
class FPSTEMPLATE_API AOWCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOWCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; 
	UAttributeSet* GetAttributeSet() const { return AttributeSet;  }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon; 

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 
};
