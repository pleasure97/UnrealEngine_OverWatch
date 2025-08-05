// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilityTargetActor/OWGATargetActor_Trace.h"
#include "OWGATargetActor_LineTrace.generated.h"

/**
 * Gameplay Ability Target Actor for Line Trace
 * Default One will destroy the TargetActor
 */
UCLASS()
class FPSTEMPLATE_API AOWGATargetActor_LineTrace : public AOWGATargetActor_Trace
{
	GENERATED_BODY()
	
public:
	AOWGATargetActor_LineTrace(); 

	/** 
	* Target Actor could be used in multiple abilities and there's no guarantees what state it will be in. 
	* You will need to make sure that only one ability is using this Target Actor at a time. 
	* 
	* @param InStartLocation - Location to Trace from 
	* @param InAimingTag (Optional) - Predicted GameplayTag for Aiming. Only Used if we Modify Spread while Aiming. If Used, Must Set InAimingRemovalTarget Also. 
	* @param InAimingRemovalTag (Optional) - Predicted GameplayTag for Aiming Removal. Only Used if we Modify Spread while Aiming. If Used, Must Set InAimingTag Also.
	* @param InTraceProfile - Collision Profile to Use for Tracing.
	* @param InFilter - Hit Actors Must Pass this Filter to be Returend in the TargetData.
	* @param InReticleClass - Reticle that will Appear on Top of Acuired Targets. Reticles will be Spawned / Despawned as Targets are Acquired / Lost. 
	* @param InReticleParams - Parameters for World Reticle. Usage of these Parameters is Dependent on the Reticle. 
	* @param bIgnoreBlockingHits - Ignore Blocking Collision Hits in the Trace. Useful if you want to Target through Walls. 
	* @param bInShouldProduceTargetDataOnServer - If Set, this Target Actor will Produce Target Data on the Server in Addition to the Client.
	*											- the Client will just send a Generic "Confirm" Event to the Server. 
	*											- If False, the Client will Send the Target Data to the Server. 
	*											- This is Handled by 'WaitTargetDataUsingActor' AbilityTask. 
	* @param bInUsePersistentHitResults - Should HitResults Persistent while Targeting? HitResults are Cleared on Confirm / Cancel or when New HitResults Take their Place
	* @param bInDebug - When true, this Target Actor will Show Debug Lines of the Trace and HitResults. 
	* @param bInTraceAffectsAimPitch - Trace can or cannot Affect the Aiming Pitch 
	* @param bInTraceFromPlayerViewpoint - Should we Trace from the Player Viewpoint instead of the StartLocation?
	*									 - Target Data HitResults will Still have the StartLocation of Trace Start. 
	*									 - This is Useful for FPS where we want to Trace from the Player Viewpoint
	*									 - But Draw the Bullet Tracer from Weapon Muzzle. 
	* @param bInUseAimingSpreadModifier - Should we Modify Spread based on if we're Aiming? 
	*									- If true, Must set InAimingTag and InAimingRemovalTag. 
	* @param InMaxRange - Max Range for this Trace.
	* @param InBaseSpread - Base Targeting Spread in Degrees. 
	* @param InAimingSpreadModifier (Optional) - Multiplicative Modifier to Spread if Aiming. 
	* @param InTargetingSpreadIncrement - Amount Spread Increments from Continuous Targeting in Degrees. 
	* @param InTargetingSpreadMax - Maximum Amount of Spread for Continuous Targeting in Degrees 
	* @param InMaxHitResulstsPerTrace - Max HitResults that a Trace can Return. If less than 1, Just Returns the Trace End Point.
	* @param InNumberOfTraces - Number of Traces to Perofrm. 
	*						  - Intended to be used with BaseSpread for Multi-Shot Weapons like Shotguns.
	*						  - Not Intended to be used with PersistentHitResults. 
	*						  - If using PersistentHitResults, NumberofTraces is Hardcoded to 1. 
	*/

	UFUNCTION(BlueprintCallable)
	void Configure(
		UPARAM(DisplayName = "Start Location") const FGameplayAbilityTargetingLocationInfo& InStartLocation,
		UPARAM(DisplayName = "Aiming Tag") FGameplayTag InAimingTag,
		UPARAM(DisplayName = "Aiming Removal Tag") FGameplayTag InAimingRemovalTag,
		UPARAM(DisplayName = "Trace Profile") FCollisionProfileName InTraceProfile,
		UPARAM(DisplayName = "Filter") FGameplayTargetDataFilterHandle InFilter,
		UPARAM(DisplayName = "Reticle Class") TSubclassOf<AGameplayAbilityWorldReticle> InReticleClass,
		UPARAM(DisplayName = "Reticle Params") FWorldReticleParameters InReticleParams,
		UPARAM(DisplayName = "Ignore Blocking Hits") bool bInIgnoreBlockingHits = false,
		UPARAM(DisplayName = "Should Produce Target Data on Server") bool bInShouldProduceTargetDataOnServer = false,
		UPARAM(DisplayName = "Use Persistent Hit Results") bool bInUsePersistentHitResults = false,
		UPARAM(DisplayName = "Debug") bool bInDebug = false,
		UPARAM(DisplayName = "Trace Affcets Aim Pitch") bool bInTraceAffectsAimPitch = true,
		UPARAM(DisplayName = "Trace from Player Viewpoint") bool bInTraceFromPlayerViewpoint = false,
		UPARAM(DisplayName = "Use Aiming Spread Modifier") bool bInUseAimingSpreadModifier = false,
		UPARAM(DisplayName = "Max Range") float InMaxRange = 999999.f,
		UPARAM(DisplayName = "Base Targeting Spread") float InBaseSpread = 0.f,
		UPARAM(DisplayName = "Aiming Spread Modifier") float InAimingSpreadModifier = 0.f,
		UPARAM(DisplayName = "Targeting Spread Increment") float InTargetingSpreadIncrement = 0.f,
		UPARAM(DisplayName = "Taregting Spread Max") float InTargetingSpreadMax = 0.f,
		UPARAM(DisplayName = "Max HitResults Per Trace") int32 InMaxHitResultsPerTrace = 1,
		UPARAM(DisplayName = "Number of Trace") int32 InNumberOfTraces = 1
	);
	
protected:
	virtual void DoTrace(
		TArray<FHitResult>& HitResults, const UWorld* World,
		const FGameplayTargetDataFilterHandle FilterHandle, 
		const FVector& Start, const FVector& End,
		FName ProfileName, const FCollisionQueryParams Params) override; 
	
	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.f) override; 
#if ENABLE_DRAW_DEBUG
	// Util for Drawing Result of Multi Line Trace from KismetTraceUtils.h
	void DrawDebugLineTraceMulti(
		const UWorld* World, const FVector& Start, const FVector& End,
		EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& OutHits,
		FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime); 
#endif
};
