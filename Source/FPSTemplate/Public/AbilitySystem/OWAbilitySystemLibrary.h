// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/Data/OmnicInfo.h"
#include "AbilitySystem/Data/HeroDebuffInfo.h"
#include "GameFramework/PlayerState.h"
#include "OWAbilitySystemLibrary.generated.h"

class UOWGameplayAbility; 
struct FWidgetControllerParams; 
class AOWHUD; 
class UOverlayWidgetController; 
struct FDamageEffectParams; 
struct FOverlapResult;
class IInteractInterface; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	 * Gameplay Abilities
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayAbilities")
	static UOWGameplayAbility* GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayAbilities")
	static UOWGameplayAbility* GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "OWAbilitySystemLibrary|GameplayAbilities")
	static bool IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle);

	/*
	 * Hero Info Defaults 
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EHeroName HeroName, UOWAbilitySystemComponent* ASC, float Level=1.f);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void ResetAttributes(const UObject* WorldContextObject, EHeroName HeroName, UOWAbilitySystemComponent* ASC, float Level = 1.f);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void GiveDefaultAbilities(const UObject* WorldContextObject, EHeroName HeroName, UAbilitySystemComponent* ASC); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static UHeroInfo* GetHeroInfo(const UObject* WorldContextObject); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static EHeroName GetHeroName(const UObject* WorldContextObject, UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static void GetIndividualHeroInfo(const UObject* WorldContextObject, EHeroName HeroName, FOWHeroInfo& OutHeroInfo); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static UAnimMontage* GetAnimMontageForTag(const UObject* WorldContextObject, EHeroName HeroName, const FGameplayTag& AnimMontageTag);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static UAnimMontage* GetAnimMontageFromAvatarActor(AActor* AvatarActor, const FGameplayTag& AnimMontageTag);

	/*
	 * Omnic Info Defaults
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|HeroInfoDefaults")
	static UOmnicInfo* GetOmnicInfo(const UObject* WorldContextObject); 

	/*
	 * Debuff Info Defaults
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|DebuffInfoDefaults")
	static UHeroDebuffInfo* GetDebuffInfo(const UObject* WorldContextObject);

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
	static FGameplayTag GetDebuffTag(const FGameplayEffectContextHandle& EffectContextHandle);

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

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin);

	/*
	 * Gameplay Mechanics 
	 */
	UFUNCTION(BlueprintCallable)
	static void SendGameplayEventToActors(AActor* Instigator, const FGameplayTag& InGameplayTag, const TArray<AActor*>& ActorsHit);

	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> CheckHitBoxOverlap(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);

	static void DrawHitBoxOverlap(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius);

	UFUNCTION(BlueprintCallable, Category="OWAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject,
		TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
		float Radius, const FVector& SphereOrigin, 
		bool bDebug = false); 

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|GameplayMechanics")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams); 

	static int32 GetXPRewardFromClassAndLevel(const UObject* WorldContextObject, EOmnicClass OmnicClass, int32 OmnicLevel); 

	/*
	 * Damage Effect Parameters 
	 */
	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|DamageEffect")
	static void SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|DamageEffect")
	static void SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "OWAbilitySystemLibrary|DamageEffect")
	static void SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InAbilitySystemComponent);

	/*
	 * Interaction
	 */
	UFUNCTION()
	static void AddInteractableActorsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IInteractInterface>>& OutInteractableActors);

	UFUNCTION()
	static void AddInteractableActorsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IInteractInterface>>& OutInteractableActors);

	UFUNCTION()
	static AActor* GetActorFromInteractableAgent(TScriptInterface<IInteractInterface> InteractableAgent);
};
