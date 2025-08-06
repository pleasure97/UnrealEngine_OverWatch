// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "Animation/OWAnimInstance.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/OWGlobalAbilitySystem.h"

bool UOWAbilitySystemComponent::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool bEndAbilityImmediately)
{
    bool bAbilityActivated = false; 
    // Check if Gameplay Ability Spec Handle is Valid 
    if (InAbilityHandle.IsValid())
    {
        // Should Use FScopedServerAbilityRPCBatcher for Ability Batching
        FScopedServerAbilityRPCBatcher ScopedServerAbilityRPCBatcher(this, InAbilityHandle); 
        // Try Activate Ability and Return bool Value 
        bAbilityActivated = TryActivateAbility(InAbilityHandle, true); 

        if (bEndAbilityImmediately)
        {
            FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InAbilityHandle);
            // Get Gameplay Ability Spec, Cast Primary Instance to Custom Gameplay Ability, and End Ability Externally  
            if (AbilitySpec)
            {
                UOWGameplayAbility* OWGameplayAbility = Cast<UOWGameplayAbility>(AbilitySpec->GetPrimaryInstance()); 
                OWGameplayAbility->ExternalEndAbility(); 
            }
        }
        return bAbilityActivated; 
    }
    return bAbilityActivated; 
}

FGameplayAbilitySpecHandle UOWAbilitySystemComponent::FindAbilitySpecHandleForClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
    // Used to stop us from removing abilities from an ability system component while we're iterating through the abilities 
    ABILITYLIST_SCOPE_LOCK(); 
    for (FGameplayAbilitySpec& GameplayAbilitySpec : ActivatableAbilities.Items)
    {
        TSubclassOf<UGameplayAbility> GameplayAbilitySpecClass = GameplayAbilitySpec.Ability->GetClass(); 
        if (GameplayAbilitySpecClass == AbilityClass)
        {
            return GameplayAbilitySpec.Handle;
        }
    }
    return FGameplayAbilitySpecHandle();
}

void UOWAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
    FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get(); 
    check(ActorInfo); 
    check(InOwnerActor); 

    const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor); 

    Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor); 

    if (bHasNewPawnAvatar)
    {
        // Register the Global Ability System.
        if (UOWGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UOWGlobalAbilitySystem>(GetWorld()))
        {
            GlobalAbilitySystem->RegisterASC(this);
        }

        // Map Ability System Component to Anim Instance 
        if (UOWAnimInstance* OWAnimInstance = Cast<UOWAnimInstance>(ActorInfo->GetAnimInstance()))
        {
            OWAnimInstance->InitializeWithAbilitySystem(this); 
        } 

        // Notify all abilities that a new pawn avatar has been set
        for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
        {
            UOWGameplayAbility* OWGameplayAbility = CastChecked<UOWGameplayAbility>(AbilitySpec.Ability);

            // Check if Instancing Policy of Gameplay Ability is Non Instanced, Instanced Per Actor, or Instanced Per Execution
            if (OWGameplayAbility->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
            {
                TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
                for (UGameplayAbility* AbilityInstance : Instances)
                {
                    UOWGameplayAbility* OWAbilityInstance = Cast<UOWGameplayAbility>(AbilityInstance);
                    if (OWAbilityInstance)
                    {
                        // Ability instances may be missing for replays
                        OWAbilityInstance->OnHeroSet();
                    }
                }
            }
            else
            {
                OWGameplayAbility->OnHeroSet();
            }
        }
    }
}

void UOWAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UOWAbilitySystemComponent::ClientEffectApplied);

    const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
}

void UOWAbilitySystemComponent::AddHeroAbilities()
{
    UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this); 
    EHeroName HeroName = UOWAbilitySystemLibrary::GetHeroName(this); 
    const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

    // Add Common Abilities
    for (const TSubclassOf<UGameplayAbility> CommonAbilityClass : HeroInfo->CommonAbilities)
    {
        FGameplayAbilitySpec CommonAbilitySpec = FGameplayAbilitySpec(CommonAbilityClass, 1 /*int32 InLevel*/);
        if (const UOWGameplayAbility* CommonGameplayAbility = Cast<UOWGameplayAbility>(CommonAbilitySpec.Ability))
        {
            CommonAbilitySpec.DynamicAbilityTags.AddTag(CommonGameplayAbility->DefaultInputTag);
        }
        CommonAbilitySpec.DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
        GiveAbility(CommonAbilitySpec);
    }

    // Add Hero's Unique Abilities 
    const TArray<FOWAbilityInfo>& UniqueAbilities = HeroInfo->HeroInformation[HeroName].Abilities; 
    for (const FOWAbilityInfo& UniqueAbility : UniqueAbilities)
    {
        const TSubclassOf<UGameplayAbility>& UniqueAbilityClass = UniqueAbility.Ability;
        FGameplayAbilitySpec UniqueAbilitySpec = FGameplayAbilitySpec(UniqueAbilityClass, 1 /*int32 InLevel*/);
        if (const UOWGameplayAbility* UniqueGameplayAbility = Cast<UOWGameplayAbility>(UniqueAbilitySpec.Ability))
        {
            UniqueAbilitySpec.DynamicAbilityTags.AddTag(UniqueGameplayAbility->DefaultInputTag);
            UniqueAbilitySpec.DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
            GiveAbility(UniqueAbilitySpec);
        }
    }
    bDefaultAbilitiesGiven = true; 
    AbilitiesGivenDelegate.Broadcast(); 
}

void UOWAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid() || HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputPressed))
    {
        return;
    }

    FScopedAbilityListLock ActiveScopeLock(*this); 
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec); 
            if (AbilitySpec.IsActive())
            {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey()); 
            }
        }
    }
}

void UOWAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid() || HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputHeld))
    {
        return;
    }

    FScopedAbilityListLock ActiveScopeLock(*this); 
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec); 
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle); 
            }
        }
    }
}

void UOWAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid() || HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Block_InputReleased))
    {
        return;
    }

    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
        {
            AbilitySpecInputReleased(AbilitySpec); 
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey()); 
        }
    }
}

void UOWAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this); 

    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to execute deleagte in %hs"), __FUNCTION__); 
        }
    }
}

FGameplayTag UOWAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for (FGameplayTag AbilityTag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
            {
                return AbilityTag; 
            }
        }
    }
    return FGameplayTag(); 
}

FGameplayTag UOWAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag InputTag : AbilitySpec.DynamicAbilityTags)
    {
        if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
        {
            return InputTag; 
        }
    }
    return FGameplayTag(); 
}

FGameplayTag UOWAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
    {
        if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
        {
            return StatusTag; 
        }
    }
    return FGameplayTag(); 
}

FGameplayTag UOWAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
    if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
    {
        return GetStatusTagFromSpec(*Spec); 
    }
    return FGameplayTag(); 
}

FGameplayAbilitySpec* UOWAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopeLock(*this); 
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (Tag.MatchesTag(AbilityTag))
            {
                return &AbilitySpec; 
            }
        }
    }
    return nullptr; 
}

void UOWAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag)
{
    if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
        const FGameplayTag& Status = GetStatusTagFromSpec(*AbilitySpec); 

        const bool bStatusValid = (Status == FOWGameplayTags::Get().Abilities_Status_Equipped); 
        if (bStatusValid)
        {
            AbilitySpec->DynamicAbilityTags.RemoveTag(GetStatusTagFromSpec(*AbilitySpec)); 
            AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped); 
            MarkAbilitySpecDirty(*AbilitySpec); 
        }
        ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped); 
    }
}

void UOWAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
    AbilityEquipped.Broadcast(AbilityTag, StatusTag); 
}

void UOWAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer GameplayTagContainer; 
    EffectSpec.GetAllAssetTags(GameplayTagContainer); 

    EffectAssetTags.Broadcast(GameplayTagContainer); 
}

void UOWAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
    ABILITYLIST_SCOPE_LOCK(); 
    for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
    {
        if (!AbilitySpec.IsActive())
        {
            continue;
        }

        UGameplayAbility* AbilityCDO = CastChecked<UGameplayAbility>(AbilitySpec.Ability); 

        if (AbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
        {
            // Cancel All Spawned Instances, not CDO. 
            TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances(); 
            for (UGameplayAbility* AbilityInstance : Instances)
            {
                if (ShouldCancelFunc(AbilityInstance, AbilitySpec.Handle))
                {
                    if (AbilityInstance->CanBeCanceled())
                    {
                        AbilityInstance->CancelAbility(
                            AbilitySpec.Handle, 
                            AbilityActorInfo.Get(), 
                            AbilityInstance->GetCurrentActivationInfo(),
                            bReplicateCancelAbility); 
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("CancelAbilitiesByFunc : Can't cancel Ability [%s]"), *AbilityInstance->GetName()); 
                    }
                }
            }
        }
    }
}

void UOWAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities(); 

    if (!bDefaultAbilitiesGiven)
    {
        bDefaultAbilitiesGiven = true; 
        AbilitiesGivenDelegate.Broadcast(); 
    }
}

void UOWAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
    AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel); 
}

void UOWAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UOWGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UOWGlobalAbilitySystem>(GetWorld()))
    {
        GlobalAbilitySystem->UnregisterASC(this); 
    }

    Super::EndPlay(EndPlayReason); 
}