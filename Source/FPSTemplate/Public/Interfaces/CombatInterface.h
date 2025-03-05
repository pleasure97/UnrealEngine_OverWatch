// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage; 
class UAbilitySystemComponent; 

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor); 


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSTEMPLATE_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* Hit React */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage(); 

	/* Ability System */
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0; 
	
	/* Death */
	virtual void Die(const FVector& DeathImpulse) = 0; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const; 

	virtual FOnDeath& GetOnDeathDelegate() = 0; 

	/* Shocked */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);
};
