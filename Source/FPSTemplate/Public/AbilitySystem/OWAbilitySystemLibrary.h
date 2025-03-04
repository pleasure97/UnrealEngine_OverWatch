// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWAbilitySystemLibrary.generated.h"

struct FWidgetControllerParams; 
class AOWHUD; 
class UOverlayWidgetController; 
class UHeroInfo; 
struct FDamageEffectParams; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*
	 * Widget Controller 
	 */

	UFUNCTION(BlueprintCallable, Category="OWAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOWHUD*& OutOWHUD); 
	
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject); 

	/*
	 * Hero Info Defaults 
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC, float Level=1.f); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void GiveDefaultAbilities(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static UHeroInfo* GetHeroInfo(const UObject* WorldContextObject); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static EHeroName GetHeroName(const UObject* WorldContextObject);

	/*
	 * Effect Context Getter
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static bool bIsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageRadius(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle); 

	/* 
	 * Effect Context Setter
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin);

	/*
	 * Gameplay Mechanics 
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayMechanics")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams); 

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InAbilitySystemComponent);
};
