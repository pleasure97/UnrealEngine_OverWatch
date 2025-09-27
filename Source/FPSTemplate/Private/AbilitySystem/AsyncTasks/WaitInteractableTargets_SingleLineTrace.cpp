// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitInteractableTargets_SingleLineTrace.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

void UWaitInteractableTargets_SingleLineTrace::Activate()
{
    // Terminate Ability Task After a Certain Period of Time 
    SetWaitingOnAvatar(); 

    if (UWorld* World = GetWorld())
    {
        // Perform Single Line Trace At Each Interaction Scan Rate 
        World->GetTimerManager().SetTimer(TimerHandle, this, &UWaitInteractableTargets_SingleLineTrace::PerformTrace, InteractionScanRate, true); 
    }
}

UWaitInteractableTargets_SingleLineTrace* UWaitInteractableTargets_SingleLineTrace::WaitInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FInteractionQuery InteractionQuery, FCollisionProfileName CollisionProfileName, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange, float InteractionScanRate, bool bShowDebug)
{
    UWaitInteractableTargets_SingleLineTrace* MyObj
        = NewAbilityTask<UWaitInteractableTargets_SingleLineTrace>(OwningAbility); 
    
    MyObj->InteractionScanRange = InteractionScanRange;
    MyObj->InteractionScanRate = InteractionScanRate;
    MyObj->StartLocation = StartLocation;
    MyObj->InteractionQuery = InteractionQuery;
    MyObj->TraceProfile = CollisionProfileName;
    MyObj->bShowDebug = bShowDebug; 

    return MyObj;
}

void UWaitInteractableTargets_SingleLineTrace::OnDestroy(bool AbilityEnded)
{
    if (UWorld* World = GetWorld())
    {
        // Clear Timer Hnadle 
        World->GetTimerManager().ClearTimer(TimerHandle);
    }

    Super::OnDestroy(AbilityEnded); 
}

void UWaitInteractableTargets_SingleLineTrace::PerformTrace()
{
    // Check if Ability is Valid
    if (Ability)
    {
        // Get Avatar Actor from Current Actor Info of Ability 
        if (AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get())
        {
            // Get World 
            if (UWorld* World = GetWorld())
            {
                // Add Self Actor to Ignored Actors
                TArray<AActor*> IgnoredActors; 
                IgnoredActors.Add(AvatarActor); 

                // Initialize Collision Query Params 
                const bool bTraceComplex = false; 
                FCollisionQueryParams CollisonQueryParams(SCENE_QUERY_STAT(UWaitInteractableTargets_SingleLineTrace), bTraceComplex); 
                CollisonQueryParams.AddIgnoredActors(IgnoredActors); 

                // Set Trace Start from Gameplay Ability Targeting Location Info 
                FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation(); 
                FVector TraceEnd; 
                AimWithPlayerController(AvatarActor, CollisonQueryParams, TraceStart, InteractionScanRange, TraceEnd); 

                // Line Trace and Save Result to Out Hit Result 
                FHitResult OutHitResult; 
                LineTrace(World, TraceStart, TraceEnd, TraceProfile.Name, CollisonQueryParams, OutHitResult);

                // Add Actor of Hit Result to Interactable Actors 
                TArray<TScriptInterface<IInteractInterface>> InteractableActors; 
                UOWAbilitySystemLibrary::AddInteractableActorsFromHitResult(OutHitResult, InteractableActors); 

                UpdateInteractionOptions(InteractionQuery, InteractableActors); 

                // Draw Line Trace and Sphere Debug
                if (bShowDebug)
                {
                    if (OutHitResult.bBlockingHit)
                    {
                        DrawDebugLine(World, TraceStart, OutHitResult.Location, FColor::Red, false, InteractionScanRate);
                        DrawDebugSphere(World, OutHitResult.Location, 5, 16, FColor::Red, false, InteractionScanRate);
                    }
                    else
                    {
                        DrawDebugLine(World, TraceStart, TraceEnd, FColor::Green, false, InteractionScanRate);
                    }
                }
            }
        }
    }
}
