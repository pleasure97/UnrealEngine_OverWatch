// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitInteractableTargets.generated.h"

struct FCollisionQueryParams;
struct FHitResult;
struct FInteractionQuery; 
class IInteractInterface; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableActorsChanged, const TArray<FInteractionOption>&, InteractableOptions); 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UWaitInteractableTargets : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FInteractableActorsChanged InteractableActorsChanged; 
	
protected:
	static void LineTrace(
		const UWorld* World, 
		const FVector& Start, const FVector& End, 
		FName ProfileName, const FCollisionQueryParams CollisionQueryParams, FHitResult& OutHitResult);
	
	void AimWithPlayerController(
		const AActor* InSourceActor, 
		FCollisionQueryParams CollisionQueryParams, 
		const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, 
		bool bIgnoresPitch = false) const; 
	
	static bool ClipCameraRayToAbilityRange(
		FVector CameraLocation, FVector CameraDirection, 
		FVector AbilityCenter, float AbilityRange,
		FVector& ClippedPosition); 

	void UpdateInteractionOptions(
		const FInteractionQuery& InteractionQuery,
		const TArray<TScriptInterface<IInteractInterface>>& InteractableActors); 

	FCollisionProfileName TraceProfile;

	// Boolean Value that the Trace Affects Aiming Pitch 
	bool bTraceAffectsAimingPitch = true; 

private:
	TArray<FInteractionOption> CurrentInteractionOptions; 
};
