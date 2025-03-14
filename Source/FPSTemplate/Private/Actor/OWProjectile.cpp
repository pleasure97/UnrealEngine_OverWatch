// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/OWProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"

AOWProjectile::AOWProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; 

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere"); 
	SetRootComponent(Sphere); 
	
	// TODO - Set Collision Object Type, ECC_Projectile 
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore); 
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); 
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap); 
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement"); 
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f; 
}

void AOWProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan); 
	SetReplicateMovement(true); 
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AOWProjectile::OnSphereOverlap); 

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent()); 
	}
}

void AOWProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop(); 
		LoopingSoundComponent->DestroyComponent(); 
	}

	if (!bHit && !HasAuthority()) OnHit(); 

	Super::Destroyed(); 
}

void AOWProjectile::OnHit()
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator); 
	}
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation()); 
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop(); 
		LoopingSoundComponent->DestroyComponent(); 
	}
	bHit = true; 
}

void AOWProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit(); 

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude; 
			DamageEffectParams.DeathImpulse = DeathImpulse; 
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC; 
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); 
		}

	}
}

bool AOWProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (!DamageEffectParams.SourceAbilitySystemComponent) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(); 
	if (SourceAvatarActor == OtherActor) return false;
	// TODO - Distinguish between Allies and Enemies 

	return true; 
}
