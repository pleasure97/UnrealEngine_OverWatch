// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage; 
class UAbilitySystemComponent; 
class USkeletalMeshComponent; 
class UAnimInstance; 

UENUM(BlueprintType)
enum EAttackDirection : uint8
{
	Left   UMETA(DisplayName = "Left"),
	Right  UMETA(DisplayName = "Right"),
	Up     UMETA(DisplayName = "Up"),
	Down   UMETA(DisplayName = "Down"),
	None   UMETA(DisplayName = "None"),
};


DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor); 
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*Damage Amount*/);

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

	/* Character Defaults */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar(); 

	/* Damage */
	virtual FOnDamageSignature& GetOnDamageSignature() = 0; 

	/* Heal */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingHealed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingHealed(bool bInHeal); 
	
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

	/* Weapon */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon(); 

	/* Camera */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TransitionCamera(bool bSmoothTransition); 

	/* Animation */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimInstance* GetFirstPersonMeshAnimInstance() const; 

	/* Attack State */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EAttackDirection GetAttackDirection() const; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAttackDirection(EAttackDirection InAttackDirection); 
};
