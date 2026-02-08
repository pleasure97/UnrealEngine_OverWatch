// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayCueNotifies/GameplayCueNotify_CaptiveSun.h"
#include "Character/IlliariCharacter.h"
#include "Actor/CaptiveSunProjectile.h"
#include "Components/SphereComponent.h"

ACaptiveSunProjectile* AGameplayCueNotify_CaptiveSun::SpawnCaptiveSun(AIlliariCharacter* Illiari)
{
	// Check if Character is Valid
	if (!IsValid(Illiari))
	{
		UE_LOG(LogTemp, Error, TEXT("Illiari Character is Not Valid in AGameplayCueNotify_CaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	if (!Illiari->IsLocallyControlled())
	{
		return nullptr;
	}

	// Get Weapon Socket Transform (First Person & Third Person)
	FTransform WeaponTransform = ICombatInterface::Execute_GetFirstPersonWeaponSocketTransform(Illiari);
	WeaponTransform.SetScale3D(FVector(1.f, 1.f, 1.f)); 

	// Get Weapon Component (First Person & Third Person)
	USceneComponent* WeaponComponent = Illiari->GetFirstPersonWeapon();
	if (!IsValid(WeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon Component is Not Valid in AGameplayCueNotify_CaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	// Spawn Actor Deferred (Captive Sun Projectile)
	ACaptiveSunProjectile* CaptiveSunProjectile = GetWorld()->SpawnActorDeferred<ACaptiveSunProjectile>(
		CaptiveSunProjectileClass, WeaponTransform, Illiari, Illiari, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(CaptiveSunProjectile))
	{
		UE_LOG(LogTemp, Error, TEXT("CaptiveSunProjectile is Not Valid in AGameplayCueNotify_CaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	CaptiveSunProjectile->SetOwner(Illiari);
	// Ignore Owner When Moving (Sphere Collision of Captive Sun)
	USphereComponent* CaptiveSunSphereCollision = Cast<USphereComponent>(CaptiveSunProjectile->GetRootComponent());
	if (!IsValid(CaptiveSunSphereCollision))
	{
		UE_LOG(LogTemp, Error, TEXT("CaptiveSun Collision is Not Valid in AGameplayCueNotify_CaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	CaptiveSunSphereCollision->SetOnlyOwnerSee(true);
	CaptiveSunSphereCollision->SetOwnerNoSee(false);

	CaptiveSunSphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	// Disable Captive Sun Collision
	CaptiveSunProjectile->SetActorEnableCollision(false); 

	// Attach Captive Sun Projectile to Weapon Component
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	CaptiveSunProjectile->AttachToComponent(WeaponComponent, AttachmentTransformRules, FName("BeamSocket"));

	CaptiveSunProjectile->FinishSpawning(CaptiveSunProjectile->GetTransform());

	return CaptiveSunProjectile;
}


