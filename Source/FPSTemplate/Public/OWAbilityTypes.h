// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "OWAbilityTypes.generated.h"

class UGameplayEffect; 
class UPackageMap; 

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {} 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGamePlayEffectClass = nullptr; 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent; 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent; 

	UPROPERTY(BlueprintReadWrite)	
	float BaseDamage = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f; 

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag(); 

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector; 

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f; 

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector; 

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector; 
};


USTRUCT(BlueprintType)
struct FPSTEMPLATE_API FOWGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/* Getter for Gameplay Effect */
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageRadius() const { return RadialDamageRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	/* Setter for Gameplay Effect */
	void SetIsCriticalHit(bool bInIsCritiaclHit) { bIsCriticalHit = bInIsCritiaclHit; }
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageRadius(float InRadialDamageRadius) { RadialDamageRadius = InRadialDamageRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
	/* Returns the actual struct used for serialization, subclasses must override this. */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct(); 
	}

	/* Creates a copy of this context, used to duplicate for later modifications. */
	virtual FOWGameplayEffectContext* Duplicate() const
	{
		FOWGameplayEffectContext* NewContext = new FOWGameplayEffectContext(); 
		*NewContext = *this; 
		if (GetHitResult())
		{
			// Does a deep copy of the hit result 
			NewContext->AddHitResult(*GetHitResult(), true); 
		}
		return NewContext; 
	}

	/* Custom Serialization, subclasses must override this. */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool bOutSuccess); 

protected:
	UPROPERTY()
	bool bIsCriticalHit = false; 

	UPROPERTY()
	bool bIsSuccessfulDebuff = false; 

	UPROPERTY()
	float DebuffDamage = 0.f; 
	
	TSharedPtr<FGameplayTag> DamageType; 

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector; 

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector; 

	UPROPERTY()
	bool bIsRadialDamage = false; 

	UPROPERTY()
	float RadialDamageRadius = 0; 

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector; 
};

template<>
struct TStructOpsTypeTraits<FOWGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FOWGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, 
		WithCopy = true
	};
};