// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/GrantNearbyInteraction.h"
#include "Interface/InteractInterface.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Interaction/InteractionOption.h"
#include "GameFramework/Controller.h"
#include "AbilitySystemComponent.h"

void UGrantNearbyInteraction::Activate()
{
    SetWaitingOnAvatar(); 

    // Call QueryInteractables() at Interaction Scan Rate Interval 
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(InteractionQueryTimerHandle, this, &UGrantNearbyInteraction::QueryInteractables, InteractionScanRate); 
    }
}

UGrantNearbyInteraction* UGrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningGameplayAbility, float InteractionScanRange, float InteractionScanRate)
{
    // Create New Ability Task - Grant Nearby Interaction
    UGrantNearbyInteraction* GrantNearbyInteraction = NewAbilityTask<UGrantNearbyInteraction>(OwningGameplayAbility);

    // Set Interaction Scan Rate and Rage of New Ability Task and Return it 
    GrantNearbyInteraction->InteractionScanRate = InteractionScanRate;
    GrantNearbyInteraction->InteractionScanRange = InteractionScanRange; 

    return GrantNearbyInteraction;
}

void UGrantNearbyInteraction::OnDestroy(bool bAbilityEnded)
{
    // Clear Interaction Query Timer Handle 
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(InteractionQueryTimerHandle); 
    }

    Super::OnDestroy(bAbilityEnded);
}

void UGrantNearbyInteraction::QueryInteractables()
{
    if (UWorld* World = GetWorld())
    {
        if (AActor* ActorOwner = GetAvatarActor())
        {
            // "SCENE_QUERY_STAT" Macro Defines Name of Trace for Future Profiling and Debugging 
            FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(UGrantNearbyInteraction), false); 

            // Declare Overlap Results Array 
            TArray<FOverlapResult> OverlapResults; 
            // Use Overlap Multi By 'Interact' Trace Channel and Determine Set of Components that Avatar Actor Overlaps 
            World->OverlapMultiByChannel(
                OverlapResults, 
                ActorOwner->GetActorLocation(), 
                FQuat::Identity,
                ECC_GameTraceChannel3 /* Interact Trace Channel*/, 
                FCollisionShape::MakeSphere(InteractionScanRange), 
                CollisionQueryParams); 

            // Check if Overlapped Components Exist
            if (OverlapResults.Num() > 0)
            {
                // Declare Array which Contains Actor Implementing Interact Interface
                TArray<TScriptInterface<IInteractInterface>> InteractableActors; 
                // Add Overlapped Interactable Actors to the Array 
                UOWAbilitySystemLibrary::AddInteractableActorsFromOverlapResults(OverlapResults, InteractableActors); 

                // Assign Requesting Avatar and Requsting Controller of Interaction Query 
                FInteractionQuery InteractionQuery; 
                InteractionQuery.RequestingAvatarActor = ActorOwner; 
                InteractionQuery.RequestingController = Cast<AController>(ActorOwner->GetOwner()); 

                // Add Interaction Options of Interactable Actors to Interaction Option Collector 
                TArray<FInteractionOption> InteractionOptions; 
                for (TScriptInterface<IInteractInterface>& InteractableActor : InteractableActors)
                {
                    FInteractionOptionCollector InteractionOptionCollector(InteractableActor, InteractionOptions); 
                    InteractableActor->AddInteractionOptions(InteractionQuery, InteractionOptionCollector); 
                }

                // Check if Any of the Options Need to Grant the Ability to User before they Can be Used 
                for (FInteractionOption& InteractionOption : InteractionOptions)
                {
                    if (InteractionOption.InteractionAbilityToGrant)
                    {
                        FObjectKey InteractionObjectKey(InteractionOption.InteractionAbilityToGrant);
                        // Give Interaction Ability using Owning Ability System Component 
                        // Save Gameplay Ability Spec Handle to Cache Map
                        if (!InteractionAbilityCache.Find(InteractionObjectKey))
                        {
                            FGameplayAbilitySpec GameplayAbilitySpec(
                                InteractionOption.InteractionAbilityToGrant, 1, INDEX_NONE, this); 
                            FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = AbilitySystemComponent->GiveAbility(GameplayAbilitySpec); 
                            InteractionAbilityCache.Add(InteractionObjectKey, GameplayAbilitySpecHandle); 
                        }
                    }
                }
            }
        }
    }
}
