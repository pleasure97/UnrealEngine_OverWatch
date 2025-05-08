// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/SolarRifle.h"
#include "AbilitySystemGlobals.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/OWProjectile.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"


void USolarRifle::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); 

	// Check Ability Cost 
	check(CurrentActorInfo); 
	if (!UAbilitySystemGlobals::Get().ShouldIgnoreCosts() && !CheckCost(CurrentSpecHandle, CurrentActorInfo)) return; 
	
	// To Hit Scan, Get Weapon's Muzzle Flash Socket Location 
	if (GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		const USkeletalMeshComponent* SolarRifle = ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo());
		const FTransform SocketTransform = SolarRifle->GetSocketTransform("MuzzleFlash");

		// Fire (Spawn Muzzle Flash, Solar Projectile, and Impact Effect if Hit)
		HitScan(SocketTransform);

		CommitAbilityCost(Handle, ActorInfo, ActivationInfo);

		CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true);

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void USolarRifle::HitScan(const FTransform& SocketTransform)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); 
	if (!bIsServer) return; 

	const FVector Start = SocketTransform.GetLocation();
	const FVector End = SocketTransform.GetRotation().GetForwardVector() * MaxDistance;

	// Hit Scan 
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	// TODO - Change Trace Channel to trace enemies
	UKismetSystemLibrary::LineTraceSingle(
		this, Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);

	// Spawn Muzzle Flash 
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleFlash, Start, SocketTransform.GetRotation().Rotator());

	// Spawn Solar Projectile Deferred 
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Start);
	SpawnTransform.SetRotation(SocketTransform.GetRotation());

	AOWProjectile* SolarProjectile = GetWorld()->SpawnActorDeferred<AOWProjectile>(
		SolarProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// If Hit 
	if (HitResult.bBlockingHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SolarProjectile->GetImpactEffect(), HitResult.Location, FRotator::ZeroRotator);

		SolarProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(HitResult.GetActor());

		// Apply Damage Effect 
		UOWAbilitySystemLibrary::ApplyDamageEffect(SolarProjectile->DamageEffectParams);
	}

	SolarProjectile->FinishSpawning(SpawnTransform);
}
