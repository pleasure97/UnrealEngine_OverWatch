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
#include "Team/OWTeamSubsystem.h"

AOWProjectile::AOWProjectile()
{
	/* Tick & Replication */
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; 
	SetReplicateMovement(true);

	/* Collision */
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere"); 
	SetRootComponent(Sphere); 
	
	// TODO - Set Collision Object Type, ECC_Projectile 
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore); 

	Sphere->SetNotifyRigidBodyCollision(true);

	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); 
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); 
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 
	if (GetOwner())
	{
		Sphere->IgnoreActorWhenMoving(GetOwner(), true);
	}

	/* Projectile Movement */
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement"); 
	ProjectileMovement->SetUpdatedComponent(Sphere);
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f; 
	ProjectileMovement->bShouldBounce = false;
}

UNiagaraSystem* AOWProjectile::GetImpactEffect() const
{
	return ImpactEffect;
}

UProjectileMovementComponent* AOWProjectile::GetProjectileMovement() const
{
	return ProjectileMovement;
}

void AOWProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(ProjectileLifeSpan); 

	bShouldAttach ? SetReplicateMovement(false) : SetReplicateMovement(true);

	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AOWProjectile::OnSphereOverlap);
		Sphere->OnComponentHit.AddDynamic(this, &AOWProjectile::OnSphereComponentHit);
	}

	if (LoopingSound)
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

	Super::Destroyed(); 
}

void AOWProjectile::OnHit(AActor* TargetActor)
{
	if (HasAuthority() && TargetActor)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			if (IsValidOverlap(TargetActor, bHealProjectile))
			{
				const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulse;
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
		}

		if (!bPenetrable || !TargetActor->IsA(APawn::StaticClass()))
		{
			Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetLifeSpan(0.1f);
		}
	}
}

void AOWProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!OtherActor || OtherActor == this || OverlappedActors.Contains(OtherActor))
	{
		return; 
	}

	OverlappedActors.Add(OtherActor);

	OnHit(OtherActor);
}

void AOWProjectile::OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		OnHit(OtherActor);
	}
	else
	{
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		}
		if (ImpactEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		}
	}
}

bool AOWProjectile::IsValidOverlap(AActor* OtherActor, bool bHeal)
{
	// Check if Source Ability System Component is Valid 
	if (!DamageEffectParams.SourceAbilitySystemComponent)
	{
		return false;
	}

	// Check if Source Avatar Actor Has been Hit 
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor(); 
	if (SourceAvatarActor == OtherActor)
	{
		return false; 
	}

	// Distinguish between Allies and Enemies 
	if (UOWTeamSubsystem* OWTeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		EOWTeamComparison TeamComparison = OWTeamSubsystem->CompareTeams(SourceAvatarActor, OtherActor); 
		switch (TeamComparison)
		{
		case EOWTeamComparison::OnSameTeam:
			return bHeal;
		case EOWTeamComparison::InvalidArgument:
			return false;
		case EOWTeamComparison::DifferentTeams:
			return !bHeal;
		default:
			return false;
		}
	}
	else
	{
		return false; 
	}
}
