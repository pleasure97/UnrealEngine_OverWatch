// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilityTargetActor/OWGATargetActor_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"

AOWGATargetActor_Trace::AOWGATargetActor_Trace()
{
	bDestroyOnConfirmation = false;
	PrimaryActorTick.bCanEverTick = true; 
	// Any item that needs the update work to be done before being ticked
	PrimaryActorTick.TickGroup = TG_PostUpdateWork; 
	/* Trace */
	MaxRange = 999999.f;
	MaxHitResultsPerTrace = 1; 
	NumberOfTraces = 1; 
	bIgnoreBlockingHits = false; 
	bTraceAffectsAimPitch = true; 
	bTraceFromPlayerViewPoint = false; 
	bUsePersistentHitResults = false; 
	/* Accuracy */
	BaseSpread = 0.f; 
	TargetingSpreadIncrement = 0.f; 
	TargetingSpreadMax = 0.f; 
	CurrentTargetingSpread = 0.f; 
	AimingSpreadModifier = 0.f;
	bUseAimingSpreadModifier = false; 
}

void AOWGATargetActor_Trace::StartTargeting(UGameplayAbility* Ability)
{
	// Don't Call Super because we can have more than One Reticle 
	SetActorTickEnabled(true); 

	OwningAbility = Ability; 
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get(); 

	// Lazy Way of Emptying and Repopulating ReticleActors
	DestroyReticleActors(); 

	if (ReticleClass)
	{
		for (int32 i = 0; i < MaxHitResultsPerTrace * NumberOfTraces; ++i)
		{
			SpawnReticleActor(GetActorLocation(), GetActorRotation()); 
		}
	}

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty(); 
	}
}

void AOWGATargetActor_Trace::CancelTargeting()
{
	const FGameplayAbilityActorInfo* ActorInfo = (OwningAbility ? OwningAbility->GetCurrentActorInfo() : nullptr); 
	UAbilitySystemComponent* ASC = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr); 
	if (ASC)
	{
		ASC->AbilityReplicatedEventDelegate(
			EAbilityGenericReplicatedEvent::GenericCancel,
			OwningAbility->GetCurrentAbilitySpecHandle(),
			OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey()
		).Remove(GenericCancelHandle); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting() called with Null Ability System Comonent! Actor %s"), *GetName()); 
	}

	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle()); 

	SetActorTickEnabled(false); 

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty(); 
	}
}

void AOWGATargetActor_Trace::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData()); 
	if (SourceActor)
	{
		TArray<FHitResult> HitResults = PerformTrace(SourceActor); 
		FGameplayAbilityTargetDataHandle GameplayAbilityTargetDataHandle = MakeTargetData(HitResults); 
		TargetDataReadyDelegate.Broadcast(GameplayAbilityTargetDataHandle);

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForDuration, 2.f); 
		}
#endif
	}

	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty(); 
	}
}

void AOWGATargetActor_Trace::StopTargeting()
{
	SetActorTickEnabled(false); 

	DestroyReticleActors(); 

	// Clear Added Callbacks
	TargetDataReadyDelegate.Clear(); 
	CanceledDelegate.Clear(); 

	if (GenericDelegateBoundASC)
	{
		GenericDelegateBoundASC->GenericLocalConfirmCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting); 
		GenericDelegateBoundASC->GenericLocalCancelCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting); 
		GenericDelegateBoundASC = nullptr; 
	}
}

void AOWGATargetActor_Trace::ResetSpread()
{
	/* Accuracy */
	BaseSpread = 0.f; 
	TargetingSpreadIncrement = 0.f; 
	TargetingSpreadMax = 0.f; 
	CurrentTargetingSpread = 0.f; 
	AimingSpreadModifier = 0.f;
	bUseAimingSpreadModifier = false;
}

float AOWGATargetActor_Trace::GetCurrentSpread() const
{
	float FinalSpread = BaseSpread + CurrentTargetingSpread; 

	if (bUseAimingSpreadModifier && (AimingTag.IsValid()) && (AimingRemovalTag.IsValid()))
	{
		UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get(); 
		if (ASC && (ASC->GetTagCount(AimingTag) > ASC->GetTagCount(AimingRemovalTag)))
		{
			FinalSpread *= AimingSpreadModifier; 
		}
	}

	return FinalSpread; 
}

void AOWGATargetActor_Trace::SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation)
{
	StartLocation = InStartLocation; 
}

void AOWGATargetActor_Trace::SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer)
{
	ShouldProduceTargetDataOnServer = bInShouldProduceTargetDataOnServer; 
}

void AOWGATargetActor_Trace::SetDestroyOnConfirmation(bool bInDestroyOnConfirmation)
{
	bDestroyOnConfirmation = bInDestroyOnConfirmation;
}

void AOWGATargetActor_Trace::BeginPlay()
{
	Super::BeginPlay(); 

	// Start with Tick Disabled. 
	// We'll Enable it in StartTargeting() and Disable it again in StopTargeting(). 
	// For Instant Confirmations, Tick will Never Happen because We StartTargeting(), ConfirmationTargeting(), and Immediately StopTargeting(). 
	SetActorTickEnabled(false); 
}

void AOWGATargetActor_Trace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyReticleActors(); 

	Super::EndPlay(EndPlayReason); 
}

void AOWGATargetActor_Trace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TArray<FHitResult> HitResults; 
	if (bDebug || bUsePersistentHitResults)
	{
		HitResults = PerformTrace(SourceActor); 
	}

#if ENABLE_DRAW_DEBUG
	if (SourceActor && bDebug)
	{
		ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForDuration);
	}
#endif
}

TArray<FHitResult> AOWGATargetActor_Trace::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore; 

	// Ignore Source Actor
	ActorsToIgnore.Add(InSourceActor); 

	FCollisionQueryParams CollsionQueryParams(SCENE_QUERY_STAT(AOWGATargetActor_LineTrace), bTraceComplex); 
	CollsionQueryParams.bReturnPhysicalMaterial = true; 
	CollsionQueryParams.AddIgnoredActors(ActorsToIgnore); 
	CollsionQueryParams.bIgnoreBlocks = bIgnoreBlockingHits; 

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation(); 
	FVector TraceEnd;

	if (PrimaryPC)
	{
		FVector ViewStart; 
		FRotator ViewRotation; 
		PrimaryPC->GetPlayerViewPoint(ViewStart, ViewRotation);

		TraceStart = bTraceFromPlayerViewPoint ? ViewStart : TraceStart; 
	}

	if (bUsePersistentHitResults)
	{
		// Clear Any Blocking Hit Results, Invalid Actors, or Actors Out of Range 
		for (int32 i = PersistentHitResults.Num() - 1; i >= 0; --i)
		{
			FHitResult& HitResult = PersistentHitResults[i]; 

			if (HitResult.bBlockingHit ||
				!IsValid(HitResult.GetActor()) ||
				FVector::DistSquared(TraceStart, HitResult.GetActor()->GetActorLocation()) > MaxRange * MaxRange)
			{
				PersistentHitResults.RemoveAt(i); 
			}
		}
	}

	TArray<FHitResult> ReturnHitResults; 

	for (int32 TraceIndex = 0; TraceIndex < NumberOfTraces; ++TraceIndex)
	{
		// Effective on Server and Launching Client Only
		AimWithPlayerController(InSourceActor, CollsionQueryParams, TraceStart, TraceEnd); 
	
		SetActorLocationAndRotation(TraceEnd, SourceActor->GetActorRotation()); 

		CurrentTraceEnd = TraceEnd; 

		TArray<FHitResult> TraceHitResults; 
		DoTrace(TraceHitResults, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, CollsionQueryParams);
	
		for (int32 j = TraceHitResults.Num() - 1; j >= 0; --j)
		{
			if ((MaxHitResultsPerTrace >= 0) && (MaxHitResultsPerTrace < j + 1))
			{
				// Trim to MaxHitResultsPerTrace
				TraceHitResults.RemoveAt(j);
				continue; 
			}

			FHitResult& HitResult = TraceHitResults[j]; 

			// If bUsePersistentHitResults, the Number of Traces is 1
			if (bUsePersistentHitResults)
			{
				// This is looping backwards so that further objects from player are added first to the queue. 
				// This results in colser actors taking precedence as the further actors will get bumped out of the Array. 
				if (IsValid(HitResult.GetActor()) && (!HitResult.bBlockingHit || PersistentHitResults.Num() < 1))
				{
					bool bActorAlreadyInPersistentHits = false; 

					// Make Sure PersistentHitResults doesn't ahve this Hit Actor Already 
					for (int32 k = 0; k < PersistentHitResults.Num(); ++k)
					{
						FHitResult& PersistentHitResult = PersistentHitResults[k]; 

						if (PersistentHitResult.GetActor() == HitResult.GetActor())
						{
							bActorAlreadyInPersistentHits = true; 
							break; 
						}
					}

					if (bActorAlreadyInPersistentHits)
					{
						continue; 
					}

					if (PersistentHitResults.Num() >= MaxHitResultsPerTrace)
					{
						// Treat PersistentHitResults like a Queue, Remove First Element
						PersistentHitResults.RemoveAt(0); 
					}

					PersistentHitResults.Add(HitResult); 
				}
			}
			else
			{
				// Reticle Actors for PersistentHitResults are Handled Later 
				int32 ReticleIndex = TraceIndex * MaxHitResultsPerTrace + j;
				if (ReticleIndex < ReticleActors.Num())
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[ReticleIndex].Get())
					{
						const bool bHitActor = (HitResult.GetActor() != nullptr); 

						if (bHitActor && !HitResult.bBlockingHit)
						{
							LocalReticleActor->SetActorHiddenInGame(false); 
							const FVector ReticleLocaton = (bHitActor && LocalReticleActor->bSnapToTargetedActor) ? HitResult.GetActor()->GetActorLocation() : HitResult.Location; 

							LocalReticleActor->SetActorLocation(ReticleLocaton); 
							LocalReticleActor->SetIsTargetAnActor(bHitActor); 
						}
						else
						{
							LocalReticleActor->SetActorHiddenInGame(true); 
						}
					}
				}
			}

			if (!bUsePersistentHitResults)
			{
				if (TraceHitResults.Num() < ReticleActors.Num())
				{
					// We have Less Hit Results than ReticleActors, Hide the Extra Ones
					for (int32 k = TraceHitResults.Num(); k < ReticleActors.Num(); ++k)
					{
						if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[j].Get())
						{
							LocalReticleActor->SetIsTargetAnActor(false); 
							LocalReticleActor->SetActorHiddenInGame(true); 
						}
					}
				}
			}

			if (TraceHitResults.Num() < 1)
			{
				// If there were No Hits, Add a Default HitResult at the End of the Trace
				FHitResult DefaultHitResult; 
				// Start Parameter could be Player Viewport. 
				// We want HitResult to Always Display the StartLocation 
				DefaultHitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();
				DefaultHitResult.TraceEnd = TraceEnd;
				DefaultHitResult.Location = TraceEnd;
				DefaultHitResult.ImpactPoint = TraceEnd;
				TraceHitResults.Add(DefaultHitResult);

				if (bUsePersistentHitResults && PersistentHitResults.Num() < 1)
				{
					PersistentHitResults.Add(DefaultHitResult);
				}
			}
		}
		ReturnHitResults.Append(TraceHitResults); 
	}

	if (bUsePersistentHitResults && (MaxHitResultsPerTrace > 0))
	{
		// Handle Reticle Actors
		for (int32 PersistentHitResultIndex = 0; PersistentHitResultIndex < PersistentHitResults.Num(); ++PersistentHitResultIndex)
		{
			FHitResult& HitResult = PersistentHitResults[PersistentHitResultIndex]; 

			// Update Trace Start because Old Persistent HitResults will have their Original Trace Start
			// and Player Could have Moved since then 
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation(); 

			if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
			{
				const bool bHitActor = (HitResult.GetActor() != nullptr); 
				if (bHitActor && !HitResult.bBlockingHit)
				{
					LocalReticleActor->SetActorHiddenInGame(false); 
					const FVector ReticleLocation = (bHitActor && (LocalReticleActor->bSnapToTargetedActor)) ? HitResult.GetActor()->GetActorLocation() : HitResult.Location; 

					LocalReticleActor->SetActorLocation(ReticleLocation); 
					LocalReticleActor->SetIsTargetAnActor(bHitActor); 
				}
				else
				{
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}

		if (PersistentHitResults.Num() < ReticleActors.Num())
		{
			// We have Less Hit Results than Reticle Actors, Hide the Extra Ones
			for (int32 PersistentHitResultIndex = PersistentHitResults.Num(); PersistentHitResultIndex < ReticleActors.Num(); ++PersistentHitResultIndex)
			{
				if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
				{
					LocalReticleActor->SetIsTargetAnActor(false); 
					LocalReticleActor->SetActorHiddenInGame(true); 
				}
			}
		}
		return PersistentHitResults; 
	}
	return ReturnHitResults; 
}

void AOWGATargetActor_Trace::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch)
{
	// Server and Launching Client Only 
	if (!OwningAbility)
	{
		return;
	}

	// Default Values in case of AI Controller
	FVector ViewStart = TraceStart; 
	FRotator ViewRotation = StartLocation.GetTargetingTransform().GetRotation().Rotator(); 

	if (PrimaryPC)
	{
		PrimaryPC->GetPlayerViewPoint(ViewStart, ViewRotation);
	}

	const FVector ViewDirection = ViewRotation.Vector();
	FVector ViewEnd = ViewStart + (ViewDirection * MaxRange); 

	ClipCameraRayToAbilityRange(ViewStart, ViewDirection, TraceStart, MaxRange, ViewEnd); 

	TArray<FHitResult> HitResults; 
	LineTraceWithFilter(HitResults, InSourceActor->GetWorld(), Filter, ViewStart, ViewEnd, TraceProfile.Name, Params); 

	CurrentTargetingSpread = FMath::Min(TargetingSpreadMax, CurrentTargetingSpread + TargetingSpreadIncrement); 

	const bool bUseTraceResult = (HitResults.Num() > 0) && (FVector::DistSquared(TraceStart, HitResults[0].Location) <= (MaxRange * MaxRange)); 

	const FVector AdjustedEnd = (bUseTraceResult) ? HitResults[0].Location : ViewEnd; 
	
	FVector AdjustedAimDirection = (AdjustedEnd - TraceStart).GetSafeNormal(); 
	if (AdjustedAimDirection.IsZero())
	{
		AdjustedAimDirection = ViewDirection; 
	}

	if (!bTraceAffectsAimPitch && bUseTraceResult)
	{
		FVector OriginalAimDirection = (ViewEnd - TraceStart).GetSafeNormal(); 

		if (!OriginalAimDirection.IsZero())
		{
			// Convert to Angles and Use Original Pitch
			const FRotator OriginalAimRotation = OriginalAimDirection.Rotation(); 

			FRotator AdjustedAimRotation = AdjustedAimDirection.Rotation(); 
			AdjustedAimRotation.Pitch = OriginalAimRotation.Pitch; 

			AdjustedAimDirection = AdjustedAimRotation.Vector(); 
		}
	}

	const float CurrentSpread = GetCurrentSpread(); 

	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f); 
	const int32 RandomSeed = FMath::Rand(); 
	FRandomStream WeaponRandomStream(RandomSeed); 
	const FVector ShootDirection = WeaponRandomStream.VRandCone(AdjustedAimDirection, ConeHalfAngle, ConeHalfAngle); 

	OutTraceEnd = TraceStart + (ShootDirection * MaxRange); 
}

bool AOWGATargetActor_Trace::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	FVector CameraToCeneter = AbilityCenter - CameraLocation; 
	float DotToCenter = FVector::DotProduct(CameraToCeneter, CameraDirection); 
	// If this Fails, we're Pointed Away from the Center
	// But we might be Inside the Sphere and able to Find a Good Exit Point. 
	if (DotToCenter >= 0)
	{
		float DistanceSquared = CameraToCeneter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange); 
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared); 
			float DistanceAlongRay = DotToCenter + DistanceFromCamera; 
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection); 
			return true; 
		}
	}
	return false;
}

void AOWGATargetActor_Trace::LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World); 

	TArray<FHitResult> HitResults; 
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params); 

	TArray<FHitResult> FilteredHitResults; 

	// Start Parameter could be Player Viewpoint. 
	// We want HitResult to Always Display the StartLocation. 
	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation(); 

	for (int32 HitIndex = 0; HitIndex < HitResults.Num(); ++HitIndex)
	{
		FHitResult& Hit = HitResults[HitIndex]; 

		if (!IsValid(Hit.GetActor()) || FilterHandle.FilterPassesForActor(Hit.GetActor()))
		{
			Hit.TraceStart = TraceStart; 
			Hit.TraceEnd = End; 

			FilteredHitResults.Add(Hit);
		}
	}

	OutHitResults = FilteredHitResults; 
}

AGameplayAbilityWorldReticle* AOWGATargetActor_Trace::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, Location, Rotation); 
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
			SpawnedReticleActor->SetActorHiddenInGame(true); 
			ReticleActors.Add(SpawnedReticleActor); 

			// Consider When Playing on a Listen Server where we are using a Replicated Reticle Actor, 
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false); 
			}

			return SpawnedReticleActor; 
		}
	}

	return nullptr;
}

void AOWGATargetActor_Trace::DestroyReticleActors()
{
	for (int32 i = ReticleActors.Num() - 1; i >= 0; --i)
	{
		if (ReticleActors[i].IsValid())
		{
			ReticleActors[i].Get()->Destroy(); 
		}
	}

	ReticleActors.Empty(); 
}

FGameplayAbilityTargetDataHandle AOWGATargetActor_Trace::MakeTargetData(const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle ReturnGameplayAbilityTargetDataHandle; 

	for (int32 i = 0; i < HitResults.Num(); ++i)
	{
		// These are Cleaned Up by FGameplayAbilityTargetDataHandle via an Internal TSharedPtr
		FGameplayAbilityTargetData_SingleTargetHit* ReturnSignleTargetHitData = new FGameplayAbilityTargetData_SingleTargetHit(); 
		ReturnSignleTargetHitData->HitResult = HitResults[i]; 
		ReturnGameplayAbilityTargetDataHandle.Add(ReturnSignleTargetHitData); 
	}
	return ReturnGameplayAbilityTargetDataHandle;
}
