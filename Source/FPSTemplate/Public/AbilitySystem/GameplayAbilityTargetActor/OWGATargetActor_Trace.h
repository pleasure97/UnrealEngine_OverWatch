// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WorldCollision.h"
#include "OWGATargetActor_Trace.generated.h"

/**
 * Gameplay Ability Target Actor for Trace
 * Default One will destroy the TargetActor 
 */
UCLASS()
class FPSTEMPLATE_API AOWGATargetActor_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AOWGATargetActor_Trace(); 

	/** Trace Member Variables**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	float MaxRange;

	// Maximum Hit Results to Return per Trace. 0 just Returns the Trace End Point. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = True), Category = "Trace")
	int32 MaxHitResultsPerTrace; 
	
	// Number of Traces to Perform at One Time. 
	// Single Bullet Weapons like Rifles will only do One Trace. 
	// Multi Bullet Weapons like Shotguns can do Multiple Traces. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = True), Category = "Trace")
	int32 NumberOfTraces; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = True), Category = "Trace")
	bool bIgnoreBlockingHits; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = True), Category = "Trace")
	bool bTraceAffectsAimPitch; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceFromPlayerViewPoint; 

	// HitResults will Persistent until Confirmation / Cancellation or until a New HitResult Takes its Place
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bUsePersistentHitResults; 
	/** Trace Member Variables End **/

	/** Accuracy Member Variables**/
	// Base Targeting Spread Degrees 
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float BaseSpread; 

	// Continuous Targeting of Spread Increment 
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadIncrement; 

	// Continuous Targeting of Max Increment
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadMax; 

	// Current Spread from Continuous Targeting
	float CurrentTargetingSpread; 

	// Aiming Spread Modifier 
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float AimingSpreadModifier; 

	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingTag; 

	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingRemovalTag;

	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	bool bUseAimingSpreadModifier; 
	/** Accuracy Member Variables End **/

	virtual void StartTargeting(UGameplayAbility* Ability) override; 
	virtual void CancelTargeting() override; 
	virtual void ConfirmTargetingAndContinue() override; 
	virtual void StopTargeting(); 

	UFUNCTION(BlueprintCallable)
	virtual void ResetSpread(); 
	virtual float GetCurrentSpread() const; 

	UFUNCTION(BlueprintCallable)
	void SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation); 

	UFUNCTION(BlueprintCallable)
	virtual void SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer); 

	UFUNCTION(BlueprintCallable)
	void SetDestroyOnConfirmation(bool bInDestroyOnConfirmation = false); 

protected:
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
	virtual void Tick(float DeltaSeconds) override; 

	virtual TArray<FHitResult> PerformTrace(AActor* InSourceActor);
	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.f) PURE_VIRTUAL(AOWGATargetActor_Trace, return; );
	virtual void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false); 
	virtual bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition); 
	// Trace as Normal, but will Manually Filter All Hit Actors 
	virtual void LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params); 
	virtual void DoTrace(TArray<FHitResult>& HitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) PURE_VIRTUAL(AOWGATargetActor_Trace, return;); 

	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors(); 

	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;

	TArray<TWeakObjectPtr<AGameplayAbilityWorldReticle>> ReticleActors; 
	TArray<FHitResult> PersistentHitResults; 
	FVector CurrentTraceEnd; 
};
