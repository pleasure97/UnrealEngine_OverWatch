// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitInteractableTargets.h"
#include "Interaction/InteractionOption.h"
#include "Interface/InteractInterface.h"
#include "AbilitySystemComponent.h"

void UWaitInteractableTargets::LineTrace(const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams CollisionQueryParams, FHitResult& OutHitResult)
{
	if (World)
	{
		// Initialize Out Hit Result 
		OutHitResult = FHitResult(); 
		// Declare Hit Results Array and Add Line Trace Results to it 
		TArray<FHitResult> HitResults; 
		World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, CollisionQueryParams); 

		OutHitResult.TraceStart = Start; 
		OutHitResult.TraceEnd = End; 

		// Use First Hit Result from Hit Results 
		if (HitResults.Num() > 0)
		{
			OutHitResult = HitResults[0]; 
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is Not Valid in UWaitInteractableTargets::LineTrace()")); 
	}
}

void UWaitInteractableTargets::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams CollisionQueryParams, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnoresPitch) const
{
	// Gameplay Ability that Created WaitInteractableTargets
	if (Ability)
	{
		// Get Player Controller from Current Actor Info of Ability
		if (APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get())
		{
			// Save View Start and View Rotator from Player Controller's View Point 
			FVector ViewStart; 
			FRotator ViewRotator; 
			PlayerController->GetPlayerViewPoint(ViewStart, ViewRotator); 

			// Set View Direction and View End 
			const FVector ViewDirection = ViewRotator.Vector();
			FVector ViewEnd = ViewStart + (ViewDirection * MaxRange); 

			ClipCameraRayToAbilityRange(ViewStart, ViewDirection, TraceStart, MaxRange, ViewEnd); 

			// Trace from View Start to Adjusted View End 
			FHitResult HitResult; 
			LineTrace(InSourceActor->GetWorld(), ViewStart, ViewDirection, TraceProfile.Name, CollisionQueryParams, HitResult); 
			// Trace Result - Check If Trace is Hit and Hit Result is in Range
			const bool bUseTraceResult = HitResult.bBlockingHit && (FVector::DistSquared(TraceStart, HitResult.Location) <= (MaxRange * MaxRange)); 
		
			// Calculate Aiming Direction
			const FVector AdjustedEnd = bUseTraceResult ? HitResult.Location : ViewEnd; 
			FVector AdjustedAimDirection = (AdjustedEnd - TraceStart).GetSafeNormal(); 
			if (AdjustedAimDirection.IsZero())
			{
				AdjustedAimDirection = ViewDirection; 
			}

			// Adjust Pitch 
			if (!bTraceAffectsAimingPitch && bUseTraceResult)
			{
				// Maintain Pitch Angle from Original Camera Ray, while Reflect Yaw Angle from Hit Result
				FVector OriginalAimDirection = (ViewEnd - TraceStart).GetSafeNormal(); 
				if (!OriginalAimDirection.IsZero())
				{
					// Convert to Angles and Use Original Pitch 
					const FRotator OriginalAimRotator = OriginalAimDirection.Rotation(); 

					FRotator AdjustedAimRotator = AdjustedAimDirection.Rotation(); 
					AdjustedAimRotator.Pitch = OriginalAimRotator.Pitch; 

					AdjustedAimDirection = AdjustedAimRotator.Vector(); 
				}
			}
			// Output Final Trace End 
			OutTraceEnd = TraceStart + (AdjustedAimDirection * MaxRange);
		}
	}
}

bool UWaitInteractableTargets::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	// Camera Location - Ray Start 
	// Camera Direction - Ray Direction 
	// Ability Center - Center of Sphere
	// Ability Range - Radius of Sphere 
	// ClippedPosition - Intersection 
	FVector CameraToCenter = AbilityCenter - CameraLocation; 
	// Distance from Center of Sphere to Ray Direction 
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	// If this Fails, we're Pointed Away from the Center
	// But we might be Inside the Sphere and able to Find a Good Exit Point. 
	if (DotToCenter >= 0)
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter); 
		float RadiusSquared = (AbilityRange * AbilityRange); 
		// If Ray Intersect with Sphere 
		if (DistanceSquared < RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared); 
			// Subtraction Instead of Addition will Get The Other Intersection Point 
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;
			// Ray Point Clipped to Sphere 
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection); 
			return true; 
		}
	}
	return false; 
}

void UWaitInteractableTargets::UpdateInteractionOptions(const FInteractionQuery& InteractionQuery, const TArray<TScriptInterface<IInteractInterface>>& InteractableActors)
{
	TArray<FInteractionOption> NewInteractionOptions;

	// Iterate Interactable Actors 
	for (const TScriptInterface<IInteractInterface>& InteractableActor : InteractableActors)
	{
		// Each Interactable Actor Fills in What Interactions are Possible with Owner of Ability 
		TArray<FInteractionOption> TempInteractionOptions; 
		FInteractionOptionCollector InteractionOptionCollector(InteractableActor, TempInteractionOptions); 
		InteractableActor->AddInteractionOptions(InteractionQuery, InteractionOptionCollector); 

		for (FInteractionOption& TempInteractionOption : TempInteractionOptions)
		{
			FGameplayAbilitySpec* InteractionAbilitySpec = nullptr; 
			if (TempInteractionOption.InteractableActorASC && TempInteractionOption.InteractableActorAbilityHandle.IsValid())
			{
				// Find Interaction Ability Spec
				InteractionAbilitySpec = TempInteractionOption.InteractableActorASC->FindAbilitySpecFromHandle(
					TempInteractionOption.InteractableActorAbilityHandle); 
			}
			// If there's an Interaction Ability, Then Activate it on ourselves
			else if (TempInteractionOption.InteractionAbilityToGrant)
			{
				// Find Interaction Ability Spec
				InteractionAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(TempInteractionOption.InteractionAbilityToGrant);
				if (InteractionAbilitySpec)
				{
					// Update Interaction Option 
					TempInteractionOption.InteractableActorASC = AbilitySystemComponent.Get(); 
					TempInteractionOption.InteractableActorAbilityHandle = InteractionAbilitySpec->Handle; 
				}
			}
			
			if (InteractionAbilitySpec)
			{
				// Check if Interaction Ability Can be Activated
				if (InteractionAbilitySpec->Ability->CanActivateAbility(InteractionAbilitySpec->Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
				{
					NewInteractionOptions.Add(TempInteractionOption); 
				}
			}
		}
	}

	bool bOptionsChanged = false;
	if (NewInteractionOptions.Num() == CurrentInteractionOptions.Num())
	{
		// < Operator is Needed to Sort New Interaction Options
		NewInteractionOptions.Sort(); 
		// If Current Interaction Options and New Interaction Options Items are Different 
		for (int InteractionOptionIndex = 0; InteractionOptionIndex < NewInteractionOptions.Num(); ++InteractionOptionIndex)
		{
			const FInteractionOption& NewInteractionOption = NewInteractionOptions[InteractionOptionIndex];
			const FInteractionOption& CurrentInteractionOption = CurrentInteractionOptions[InteractionOptionIndex]; 

			// Compare Two Interaction Option using Custom Comparison Operator 
			if (NewInteractionOption != CurrentInteractionOption)
			{
				bOptionsChanged = true; 
				break;
			}
		}
	}
	else
	{
		bOptionsChanged = true; 
	}

	// If Options are Changed, then Save New Interaction Options and Broadcast them
	if (bOptionsChanged)
	{
		CurrentInteractionOptions = NewInteractionOptions; 
		InteractableActorsChanged.Broadcast(CurrentInteractionOptions); 
	}
}
