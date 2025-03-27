// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/CaptiveSun.h"
#include "AbilitySystemGlobals.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/CaptiveSunProjectile.h"
#include "OWGameplayTags.h"

void UCaptiveSun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	// Check Ability Cost 
	check(CurrentActorInfo); 
	if (!UAbilitySystemGlobals::Get().ShouldIgnoreCosts() && !CheckCost(CurrentSpecHandle, CurrentActorInfo)) return; 

	// Set Owner Character
	OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()); 

	// Commit Ability Cost
	CommitAbilityCost(Handle, ActorInfo, ActivationInfo); 

	// Soaring 
	if (VelocityCurve)
	{
		FOnTimelineFloat TimelineProgress; 
		TimelineProgress.BindUFunction(this, FName("UpdateSoaringVelocity")); 
		SoaringVelocity->AddInterpFloat(VelocityCurve, TimelineProgress); 

		FOnTimelineEvent TimelineFinished; 
		TimelineFinished.BindUFunction(this, FName("FinishSoaring")); 
		SoaringVelocity->SetTimelineFinishedFunc(TimelineFinished); 

		SoaringVelocity->SetLooping(false); 
		SoaringVelocity->PlayFromStart(); 
	}

	SpawnCaptiveSun(); 

	// Wait Input Press
	UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this); 
	if (WaitInputPressTask)
	{
		WaitInputPressTask->OnPress.AddDynamic(this, &UCaptiveSun::Shoot); 
	}
}

void UCaptiveSun::Shoot(float TimeWaited)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
}

void UCaptiveSun::UpdateSoaringVelocity(float Velocity)
{
	OwnerCharacter->LaunchCharacter(FVector(0., 0., Velocity), true, true); 
}

void UCaptiveSun::FinishSoaring()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying); 
	OwnerCharacter->GetCharacterMovement()->GravityScale = 0.f; 
}

void UCaptiveSun::SpawnCaptiveSun()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); 
	if (!bIsServer) return;

	FTransform SpawnTransform; 
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* SolarRifle = ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo());
		// TODO - RTS Parent Bone Space
		SpawnTransform = SolarRifle->GetSocketTransform(FName("BeamSocket")); 

		// Spawn Captive Sun Deferred
		CaptiveSunProjectile = GetWorld()->SpawnActorDeferred<ACaptiveSunProjectile>(
			CaptiveSunProjectileClass,
			SpawnTransform,
			OwnerCharacter,
			OwnerCharacter,
			ESpawnActorCollisionHandlingMethod::Undefined,
			ESpawnActorScaleMethod::OverrideRootScale);

		CaptiveSunProjectile->SetActorEnableCollision(false);
		CaptiveSunProjectile->AttachToComponent(SolarRifle, FAttachmentTransformRules::KeepRelativeTransform, FName("BeamSocket")); 
		CaptiveSunProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(nullptr, FOWGameplayTags::Get().Debuff_Laceration);
		CaptiveSunProjectile->FinishSpawning(SpawnTransform);
	}
}
