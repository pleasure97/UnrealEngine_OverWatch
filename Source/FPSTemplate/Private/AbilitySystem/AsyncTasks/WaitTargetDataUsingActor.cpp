// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitTargetDataUsingActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GameplayAbilityTargetActor/OWGATargetActor_Trace.h"

UWaitTargetDataUsingActor::UWaitTargetDataUsingActor(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UWaitTargetDataUsingActor* UWaitTargetDataUsingActor::WaitTargetDataWithReusableActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, TEnumAsByte<EGameplayTargetingConfirmation::Type> InConfirmationType, AGameplayAbilityTargetActor* InTargetActor, bool bInCreateKeyIfNotValidForMorePredicting)
{
    UWaitTargetDataUsingActor* MyObj = NewAbilityTask<UWaitTargetDataUsingActor>(OwningAbility, TaskInstanceName); 
    MyObj->TargetActor = InTargetActor; 
    MyObj->ConfirmationType = InConfirmationType;
    MyObj->bCreateKeyIfNotValidForMorePredicting = bInCreateKeyIfNotValidForMorePredicting;
    
    return MyObj; 
}

void UWaitTargetDataUsingActor::Activate()
{
    if (!IsValid(this))
    {
        return;
    }

    if (Ability && TargetActor)
    {
        InitializeTargetActor(); 
        RegisterTargetDataCallbacks(); 
        FinalizeTargetActor(); 
    }
    else
    {
        EndTask(); 
    }
}

void UWaitTargetDataUsingActor::OnTargetDataReplicated(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag ActivationTag)
{
    check(AbilitySystemComponent.Get()); 

    FGameplayAbilityTargetDataHandle MutableGameplayAbilityTargetDataHandle = GameplayAbilityTargetDataHandle; 
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); 

    // Call into the Target Actor to Sanitize / Verify the Data. 
    // If this Returns False, we are Rejecting the Replicated Target Data and will Treat this as a Cancel. 
    // This can Also be Used for Bandwidth Optimizations. 
    // OnReplicatedTargetDataReceived could do an Actual Trace and Check whatever Server Side and Use that Data. 
    // So rather than Having the Client Send that Data Explicitly, 
    // the Client is Basically Just Sending a 'Confirm' and the Server is now going to do the Work in OnReplicatedTargetDataReceived. 
    if (TargetActor && (!TargetActor->OnReplicatedTargetDataReceived(MutableGameplayAbilityTargetDataHandle)))
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            Canceled.Broadcast(MutableGameplayAbilityTargetDataHandle); 
        }
    }
    else
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            ValidData.Broadcast(MutableGameplayAbilityTargetDataHandle); 
        }
    }

    if (ConfirmationType != EGameplayTargetingConfirmation::CustomMulti)
    {
        EndTask(); 
    }

}

void UWaitTargetDataUsingActor::OnTargetDataReplicatedCanceled()
{
    check(AbilitySystemComponent.Get()); 

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        Canceled.Broadcast(FGameplayAbilityTargetDataHandle()); 
    }

    EndTask(); 
}

void UWaitTargetDataUsingActor::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle)
{
    check(AbilitySystemComponent.Get()); 

    if (!Ability)
    {
        return;
    }

    FScopedPredictionWindow ScopedPredictionWindow(
        AbilitySystemComponent.Get(),
        ShouldReplicateDataToServer() && (bCreateKeyIfNotValidForMorePredicting && !AbilitySystemComponent->ScopedPredictionKey.IsValidForMorePrediction())); 

    const FGameplayAbilityActorInfo* GameplayAbilityActorInfo = Ability->GetCurrentActorInfo(); 
    if (IsPredictingClient())
    {
        if (!TargetActor->ShouldProduceTargetDataOnServer)
        {
            FGameplayTag ApplicationTag; 
            AbilitySystemComponent->CallServerSetReplicatedTargetData(
                GetAbilitySpecHandle(),
                GetActivationPredictionKey(),
                GameplayAbilityTargetDataHandle,
                ApplicationTag,
                AbilitySystemComponent->ScopedPredictionKey); 
        }
        else if (ConfirmationType == EGameplayTargetingConfirmation::UserConfirmed)
        {
            // We arent' going to send the Target Data, but we will send a Generic Confirmed Message. 
            AbilitySystemComponent->ServerSetReplicatedEvent(
                EAbilityGenericReplicatedEvent::GenericConfirm,
                GetAbilitySpecHandle(),
                GetActivationPredictionKey(), 
                AbilitySystemComponent->ScopedPredictionKey);
        }
    }

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        ValidData.Broadcast(GameplayAbilityTargetDataHandle); 
    }

    if (ConfirmationType != EGameplayTargetingConfirmation::CustomMulti)
    {
        EndTask(); 
    }

}

void UWaitTargetDataUsingActor::OnTargetDataCanceled(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle)
{
    check(AbilitySystemComponent.Get());

    FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get(), IsPredictingClient());

    if (IsPredictingClient())
    {
        if (!TargetActor->ShouldProduceTargetDataOnServer)
        {
            AbilitySystemComponent->ServerSetReplicatedTargetDataCancelled(
                GetAbilitySpecHandle(),
                GetActivationPredictionKey(),
                AbilitySystemComponent->ScopedPredictionKey); 
        }
        else
        {
            AbilitySystemComponent->ServerSetReplicatedEvent(
                EAbilityGenericReplicatedEvent::GenericCancel,
                GetAbilitySpecHandle(),
                GetActivationPredictionKey(),
                AbilitySystemComponent->ScopedPredictionKey); 
        }
    }

    Canceled.Broadcast(GameplayAbilityTargetDataHandle); 

    EndTask();
}

void UWaitTargetDataUsingActor::ExternalConfirm(bool bEndTask)
{
    check(AbilitySystemComponent.Get()); 

    if (TargetActor)
    {
        if (TargetActor->ShouldProduceTargetData())
        {
            TargetActor->ConfirmTargetingAndContinue(); 
        }
    }

    Super::ExternalConfirm(bEndTask);
}

void UWaitTargetDataUsingActor::ExternalCancel()
{
    check(AbilitySystemComponent.Get());

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        Canceled.Broadcast(FGameplayAbilityTargetDataHandle()); 
    }

    Super::ExternalCancel(); 
}

void UWaitTargetDataUsingActor::OnDestroy(bool bAbilityEnded)
{
    if (TargetActor)
    {
        AOWGATargetActor_Trace* TraceTargetActor = Cast<AOWGATargetActor_Trace>(TargetActor); 
        if (TraceTargetActor)
        {
            TraceTargetActor->StopTargeting(); 
        }
        else
        {
            // Target Actor doesn't have a StopTargeting() 
            TargetActor->SetActorTickEnabled(false); 

            // Clear Added Callbacks
            TargetActor->TargetDataReadyDelegate.RemoveAll(this); 
            TargetActor->CanceledDelegate.RemoveAll(this);

            AbilitySystemComponent->GenericLocalConfirmCallbacks.RemoveDynamic(TargetActor, &AGameplayAbilityTargetActor::ConfirmTargeting); 
            AbilitySystemComponent->GenericLocalCancelCallbacks.RemoveDynamic(TargetActor, &AGameplayAbilityTargetActor::CancelTargeting);

            TargetActor->GenericDelegateBoundASC = nullptr; 
        }
    }

    Super::OnDestroy(bAbilityEnded);
}

void UWaitTargetDataUsingActor::InitializeTargetActor() const
{
    check(TargetActor); 
    check(Ability); 

    TargetActor->PrimaryPC = Ability->GetCurrentActorInfo()->PlayerController.Get(); 

    // If we Spawned the Target Actor, Always Register the Callbacks for when the Data is Ready.  
    TargetActor->TargetDataReadyDelegate.AddUObject(const_cast<UWaitTargetDataUsingActor*>(this), &UWaitTargetDataUsingActor::OnTargetDataReady); 
    TargetActor->CanceledDelegate.AddUObject(const_cast<UWaitTargetDataUsingActor*>(this), &UWaitTargetDataUsingActor::OnTargetDataCanceled);
}

void UWaitTargetDataUsingActor::FinalizeTargetActor() const
{
    check(TargetActor); 
    check(Ability); 

    TargetActor->StartTargeting(Ability); 
    if (TargetActor->ShouldProduceTargetData())
    {
        // If Instant Confirm, then Stop Targeting Immediately. 
        if (ConfirmationType == EGameplayTargetingConfirmation::Instant)
        {
            TargetActor->ConfirmTargeting(); 
        }
        else if (ConfirmationType == EGameplayTargetingConfirmation::UserConfirmed)
        {
            // Bind to Cancel / Confirm Delegates (Called from Local Confirm or Replicated Confirm)
            TargetActor->BindToConfirmCancelInputs();
        }
    }
}

void UWaitTargetDataUsingActor::RegisterTargetDataCallbacks()
{
    if (!IsValid(this))
    {
        return;
    }

    check(Ability); 

    const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
    const bool bShouldProduceTargetDataOnServer = TargetActor->ShouldProduceTargetDataOnServer; 

    // If Not Locally Controlled (Server for Remote Client), 
    // See if TargetData was Already Sent, else Register Callback for when it does Get Here
    if (!bIsLocallyControlled)
    {
        // Register with Target Data Callbacks if we are Expecting Client to Send them
        if (!bShouldProduceTargetDataOnServer)
        {
            FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = GetAbilitySpecHandle(); 
            FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); 

            // Since MultiFire is Supported, we still Need to Hook up the Callbacks. 
            AbilitySystemComponent->AbilityTargetDataSetDelegate(GameplayAbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UWaitTargetDataUsingActor::OnTargetDataReplicated); 
            AbilitySystemComponent->AbilityTargetDataCancelledDelegate(GameplayAbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UWaitTargetDataUsingActor::OnTargetDataReplicatedCanceled); 

            AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GameplayAbilitySpecHandle, ActivationPredictionKey);
        
            SetWaitingOnRemotePlayerData();
        }
    }
}

bool UWaitTargetDataUsingActor::ShouldReplicateDataToServer() const
{
    if (!Ability || !TargetActor)
    {
        return false;
    }

    // Send Target Data to the Server if we are the Client and this isn't a GameplayTagActor that can Produce Data on the Server
    const FGameplayAbilityActorInfo* GameplayAbilityActorInfo = Ability->GetCurrentActorInfo(); 
    if (!GameplayAbilityActorInfo->IsNetAuthority() && !TargetActor->ShouldProduceTargetDataOnServer)
    {
        return true; 
    }

    return false; 
}

