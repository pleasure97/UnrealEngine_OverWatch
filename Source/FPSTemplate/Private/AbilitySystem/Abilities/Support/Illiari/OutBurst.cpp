// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/OutBurst.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "OWGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UOutBurst::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	// Play Montage and Wait 
	if (OutburstMontage)
	{
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), OutburstMontage); 
	}

	// Wait Input Release
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this); 
	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->OnRelease.AddDynamic(this, &UOutBurst::OnInputRelease); 
	}
}

void UOutBurst::PrepareToEndAbility()
{
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOutBurst::OnInputRelease(float TimeHeld)
{
	Soar(TimeHeld); 

	TArray<AActor*> TargetEnemies; 
	TraceEnemies(TargetEnemies);

	for (AActor* TargetEnemy : TargetEnemies)
	{
		DamageAndKnockback(TargetEnemy);
	}

	PrepareToEndAbility(); 
}

void UOutBurst::Soar(float TimeHeld)
{
	// Check Avatar Actor Has Authority 
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return; 

	// Clamp TimeHeld from 0 to max. 
	float HeldTime =  UKismetMathLibrary::Clamp(TimeHeld, 0.f, MaxHeldTime); 
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()); 

	// Launch Character 
	FVector LaunchVelocity = (OwnerCharacter->GetActorForwardVector() + FVector(0.f, 0.f, 1.f)) * MaxOutburstVelocity * (TimeHeld / MaxHeldTime); 
	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true); 

	// Execute Gameplay Cue With Parameters on Owner
	FGameplayCueParameters OwnerCueParameters = UAbilitySystemBlueprintLibrary::MakeGameplayCueParameters(
		0.f, 0.f, FGameplayEffectContextHandle(), FGameplayTag(), FGameplayTag(), FGameplayTagContainer(), FGameplayTagContainer(),
		OwnerCharacter->GetActorLocation(), FVector::ZeroVector, nullptr, nullptr, nullptr, nullptr, 1, 1, nullptr, false);

	check(CurrentActorInfo);
	const_cast<FGameplayCueParameters&>(OwnerCueParameters).AbilityLevel = GetAbilityLevel();

	UAbilitySystemComponent* const AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
	AbilitySystemComponent->ExecuteGameplayCue(FOWGameplayTags::Get().GameplayCue_Illiari_Outburst, OwnerCueParameters);
}

void UOutBurst::TraceEnemies(TArray<AActor*>& TargetEnemies)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo(); 
	// Radius Origin based on Avatar Actor's Location 
	const FVector& RadiusOrigin = AvatarActor->GetActorLocation(); 

	TArray<AActor*> IgnoredActors; 
	IgnoredActors.Add(AvatarActor); 

	// Find Live Players Within Radius 
	UOWAbilitySystemLibrary::GetLivePlayersWithinRadius(
		AvatarActor,TargetEnemies, IgnoredActors, RadialDamageRadius, RadiusOrigin); 
}

void UOutBurst::DamageAndKnockback(AActor* TargetActor)
{
	const FVector& RadiusOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor, RadiusOrigin,
		true, TargetActor->GetActorLocation() - RadiusOrigin, 
		true, TargetActor->GetActorLocation() - RadiusOrigin, 
		true, 20.f); 

	UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); 
}
