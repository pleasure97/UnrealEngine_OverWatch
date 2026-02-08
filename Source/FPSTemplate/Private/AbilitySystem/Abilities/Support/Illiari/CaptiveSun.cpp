// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Support/Illiari/CaptiveSun.h"
#include "Character/IlliariCharacter.h"
#include "Actor/CaptiveSunProjectile.h"
#include "Components/SphereComponent.h"

ACaptiveSunProjectile* UCaptiveSun::SpawnCaptiveSun(AIlliariCharacter* Illiari)
{
	// Check if Character is Valid
	if (!IsValid(Illiari))
	{
		UE_LOG(LogTemp, Error, TEXT("Illiari Character is Not Valid in UCaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	// Get Weapon Socket Transform (First Person & Third Person)
	FTransform WeaponTransform = ICombatInterface::Execute_GetThirdPersonWeaponSocketTransform(Illiari);
	WeaponTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	// Get Weapon Component (First Person & Third Person)
	USceneComponent* WeaponComponent = Illiari->GetThirdPersonWeapon();
	if (!IsValid(WeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon Component is Not Valid in UCaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	// Spawn Actor Deferred (Captive Sun Projectile)
	ACaptiveSunProjectile* CaptiveSunProjectile = GetWorld()->SpawnActorDeferred<ACaptiveSunProjectile>(
		CaptiveSunProjectileClass, WeaponTransform, Illiari, Illiari, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(CaptiveSunProjectile))
	{
		UE_LOG(LogTemp, Error, TEXT("CaptiveSunProjectile is Not Valid in UCaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	// Ignore Owner When Moving (Sphere Collision of Captive Sun)
	USphereComponent* CaptiveSunSphereCollision = Cast<USphereComponent>(CaptiveSunProjectile->GetRootComponent());
	if (!IsValid(CaptiveSunSphereCollision))
	{
		UE_LOG(LogTemp, Error, TEXT("CaptiveSun Collision is Not Valid in UCaptiveSun::SpawnCaptiveSun()"));
		return nullptr;
	}

	// Prevent Captive Sun from Being Seen in First Person View
	CaptiveSunProjectile->SetOwner(Illiari);
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CaptiveSunProjectile->GetComponents<UPrimitiveComponent>(PrimitiveComponents);
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		PrimitiveComponent->SetOwnerNoSee(true);
	}

	CaptiveSunSphereCollision->IgnoreActorWhenMoving(Illiari, true);
	// Disable Captive Sun Collision
	CaptiveSunProjectile->SetActorEnableCollision(false);

	// Attach Captive Sun Projectile to Weapon Component
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	CaptiveSunProjectile->AttachToComponent(WeaponComponent, AttachmentTransformRules, FName("BeamSocket"));

	CaptiveSunProjectile->FinishSpawning(CaptiveSunProjectile->GetTransform());

	return CaptiveSunProjectile;
}
