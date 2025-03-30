// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/HealingRay.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "OWGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"

void UHealingRay::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	// Check Ability Cost 
	check(CurrentActorInfo); 
	if (!UAbilitySystemGlobals::Get().ShouldIgnoreCosts() && !CheckCost(CurrentSpecHandle, CurrentActorInfo)) return; 
	
	// Store Owner Character 
	OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor); 

	// Check Avatar Actor is not nullptr and whether implements combat interfcae 
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->Implements<UCombatInterface>()) return;

	// Execute Gameplay Cue on Owner 
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked(); 
	AbilitySystemComponent->ExecuteGameplayCue(FOWGameplayTags::Get().GameplayCue_Illiari_HealingRay, FGameplayEffectContextHandle());

	// Play Montage and Wait
	if (HealingRayMontage)
	{
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), HealingRayMontage);
	}

	// Wait Gameplay Event
	UAbilityAsync_WaitGameplayEvent* WaitGameplayEventTask = UAbilityAsync_WaitGameplayEvent::WaitGameplayEventToActor(
		GetAvatarActorFromActorInfo(), FOWGameplayTags::Get().Event_Montage_HealingRay, true, true);
	if (WaitGameplayEventTask)
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UHealingRay::OnGameplayEventReceived); 
	}

	// Wait Input Release
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask =UAbilityTask_WaitInputRelease::WaitInputRelease(this); 
	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->OnRelease.AddDynamic(this, &UHealingRay::OnInputRelease); 
	}
}

void UHealingRay::PrepareToEndAbility()
{
	// Set Movement Mode to Walking
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); 
	if (bTargetImplementsInterface)
	{
		ICombatInterface::Execute_SetIsBeingHealed(HealedTarget, false); 
		// Remove Gameplay Cue of Healing Ray 
		UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(HealedTarget, FOWGameplayTags::Get().GameplayCue_Illiari_HealingRay, TargetCueParameters); 
		// Check Authority and Apply Damage Effect
		if (HasAuthority(&CurrentActivationInfo))
		{
			UOWAbilitySystemLibrary::ApplyDamageEffect(MakeDamageEffectParamsFromClassDefaults(HealedTarget));
		}
	}
}

void UHealingRay::OnGameplayEventReceived(FGameplayEventData Payload)
{
	OwnerCharacter->GetCharacterMovement()->DisableMovement(); 
	SpawnHealBeam(); 
}

void UHealingRay::OnInputRelease(float TimeHeld)
{
	HealElapsedSeconds = TimeHeld; 
	if (HasAuthority(&CurrentActivationInfo))
	{
		ClearTimerAndEndAbility(); 
	}
	else
	{
		PrepareToEndAbility(); 
	}
}

void UHealingRay::SpawnHealBeam()
{
	check(OwnerCharacter); 
	if (!OwnerCharacter->Implements<UCombatInterface>()) return; 

	USkeletalMeshComponent* SolarRifle = ICombatInterface::Execute_GetWeapon(OwnerCharacter);
	const FTransform SocketTransform = SolarRifle->GetSocketTransform("BeamSocket");

	const FVector Start = SocketTransform.GetLocation();
	const FVector End = SocketTransform.GetRotation().GetForwardVector() * MaxDistance;

	// Line Trace from Solar Rifle's Socket 
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(OwnerCharacter); 
	// TODO - Change Trace Channel to trace allies 
	UKismetSystemLibrary::LineTraceSingle(
		OwnerCharacter, Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);

	const FVector HitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : FVector::ZeroVector; 
	AActor* HitActor = HitResult.bBlockingHit ? HitResult.GetActor() : nullptr; 

	// Make Gameplay Cue Parameters - Location: HitLocation, Source Object: HitActor, Target Attach Component: Solar Rifle
	TargetCueParameters = UAbilitySystemBlueprintLibrary::MakeGameplayCueParameters(
		0.f, 0.f, FGameplayEffectContextHandle(), FGameplayTag(), FGameplayTag(), FGameplayTagContainer(), FGameplayTagContainer(),
		HitLocation, FVector::ZeroVector, nullptr, nullptr, HitActor, nullptr, 1, 1, SolarRifle, false); 

	// Check Line Trace's Hit Result is an Actor or an Enemy
	
	if (HitActor != nullptr && HitActor->Implements<UCombatInterface>())
	{
		HealedTarget = HitActor;
		bTargetImplementsInterface = true; 
	}
	else
	{
		HealedTarget = GetAvatarActorFromActorInfo(); 
		bTargetImplementsInterface = false; 
	}
	UGameplayCueFunctionLibrary::AddGameplayCueOnActor(HealedTarget, FOWGameplayTags::Get().GameplayCue_Illiari_HealingRay, TargetCueParameters); 

	if (bTargetImplementsInterface)
	{
		ICombatInterface::Execute_SetIsBeingHealed(HealedTarget, true); 
	}

	//// TODO - Check Location and Rotation 
	//// Spawn Heal Laser from Solar Rifle's Socket 
	//// Adjust Heal Laser's Life time according to Elapsed Time 
	//check(HealLaser);
	//UNiagaraComponent* HealLaserComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HealLaser, Start);
	//if (HealLaserComponent)
	//{
	//	HealLaserComponent->SetVectorParameter("BeamStart", Start);
	//	HealLaserComponent->SetFloatParameter("ParticleLifetime", HealElapsedSeconds * 0.8f);
	//	HealLaserComponent->SetFloatParameter("LoopDuration", HealElapsedSeconds * 0.8f);
	//}

	//// When Allies are Hit by Healing Ray, Spawn Heal Laser Impact 
	//if (HitResult.bBlockingHit)
	//{
	//	HealLaserComponent->SetVectorParameter("BeamEnd", HitResult.ImpactPoint);
	//	UNiagaraComponent* HealLaserImpactComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HealLaserImpact, HitResult.ImpactPoint);
	//	if (HealLaserImpactComponent)
	//	{
	//		HealLaserImpactComponent->SetFloatParameter("ParticleLifetime", HealElapsedSeconds * 0.8f);
	//		HealLaserImpactComponent->SetFloatParameter("LoopDuration", HealElapsedSeconds * 0.8f);
	//	}
	//	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(HitResult.GetActor());
	//	UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	//}
	//else
	//{
	//	HealLaserComponent->SetVectorParameter("BeamEnd", End);
	//}

	LastHealTime = GetWorld()->GetTimeSeconds(); 

	// Heal Gauge Timer 
	if (!GetWorld()->GetTimerManager().IsTimerActive(HealGaugeTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(HealGaugeTimerHandle, this, &UHealingRay::HealAndCost, TimerRate, true); 
	}

	// Heal Regeneration Timer 
	if (!GetWorld()->GetTimerManager().IsTimerActive(HealRegenTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(HealRegenTimerHandle, this, &UHealingRay::RegenerateHealGauage, TimerRate, true); 
	}
}

void UHealingRay::HealAndCost()
{
	if (HealGauge >= 0.f)
	{
		HealGauge -= TimerRate * HealAmount; 

		if (!IsValid(HealedTarget)) return; 

		ApplyHeal(HealedTarget); 
	}
	else
	{
		ClearTimerAndEndAbility();
	}
}

void UHealingRay::ClearTimerAndEndAbility()
{
	// Clear Timer 
	GetWorld()->GetTimerManager().ClearTimer(HealGaugeTimerHandle);
	PrepareToEndAbility();
	// Commit Cooldown 
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	// Reset States
	HealedTarget = nullptr;
	bTargetImplementsInterface = false;
	HealElapsedSeconds = 0.f;
	// End Ability 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHealingRay::ApplyHeal(AActor* TargetActor)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC) return;

	FGameplayEffectSpecHandle GameplayEffectSpecHandle =
		SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle, FOWGameplayTags::Get().Damage_Healing, GetDamageAtLevel()); 

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); 
	if (IsValid(TargetASC))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	}
}

void UHealingRay::RegenerateHealGauage()
{
	float CurrentTime = GetWorld()->GetTimeSeconds(); 

	if (CurrentTime - LastHealTime < 1.f) return; 

	HealGauge += HealRegeneration * TimerRate; 
	HealGauge = FMath::Min(HealGauge, MaxHealGauge); 

	if (HealGauge >= MaxHealGauge)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealRegenTimerHandle); 
	}
}
