// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CaptiveSunProjectile.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team/OWTeamSubsystem.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "OWGameplayTags.h"
#include "Character/IlliariCharacter.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"

void ACaptiveSunProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!OtherActor || OtherActor == this || OverlappedActors.Contains(OtherActor) || OtherActor == GetOwner())
	{
		return;
	}

	OverlappedActors.Add(OtherActor);

	OnHit(OtherActor);
}

void ACaptiveSunProjectile::Shoot()
{
	if (!HasAuthority())
	{
		return;
	}

	// Get Owner Character and Check if Owner Character is Valid
	AOWCharacter* OWCharacter = Cast<AOWCharacter>(GetOwner()); 
	if (!IsValid(OWCharacter))
	{
		return;
	}

	// Get Owning Player Controller and Check if Owning Player Controller is Valid
	APlayerController* OWPlayerController = Cast<APlayerController>(OWCharacter->GetController());
	if (!IsValid(OWPlayerController))
	{
		return;
	}
	
	// Get Location and Rotation of Player View Point
	FVector CameraLocation;
	FRotator CameraRotation;
	OWPlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Setup Trace Start, Trace End, Ignored Actors 
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * ProjectileVelocity * ProjectileLifeSpan);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OWCharacter);
	Params.AddIgnoredActor(this);

	// Single Line Trace (Visibility)
	FVector TargetPoint;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		TargetPoint = HitResult.ImpactPoint;
	}
	else
	{
		TargetPoint = TraceEnd;
	}

	// Calculate Error between Target Point and Socket Location
	FVector LaunchDirection = (TargetPoint - GetActorLocation()).GetSafeNormal();

	// Shoot
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	SetActorRotation(CameraRotation);

	// Activate Projectile Movement 
	SetReplicateMovement(true);
	ProjectileMovement->Velocity = LaunchDirection * ProjectileVelocity;
	ProjectileMovement->Activate();

	// Enable Collision
	SetActorEnableCollision(true);

	Sphere->OnComponentHit.AddDynamic(this, &ACaptiveSunProjectile::BurstWhenHit);

	// Reset Projectile Life Span
	SetLifeSpan(ProjectileLifeSpan);
}

void ACaptiveSunProjectile::BurstWhenHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHasExploded)
	{
		return;
	}

	Burst(Hit.Location, OtherActor);
	
	bHasExploded = true;
}

void ACaptiveSunProjectile::Burst(const FVector& ProjectileLocation, AActor* DirectHitActor)
{
	// Check Has Authority
	if (!HasAuthority())
	{
		return;
	}

	// Get Owner and Cast Ability System Interface 
	AActor* OwnerActor = GetOwner();
	IAbilitySystemInterface* OwnerWithASC = Cast<IAbilitySystemInterface>(OwnerActor);

	// Check if Owner Actor and Direct Hit Actor are Valid
	if (!IsValid(OwnerActor) || !OwnerWithASC)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is Not Valid in ACaptiveSunProjectile::Burst()"));
		return;
	}

	// check if Owner Ability System Component is Valid
	UAbilitySystemComponent* OwnerASC = OwnerWithASC->GetAbilitySystemComponent();
	if (!IsValid(OwnerASC))
	{
		UE_LOG(LogTemp, Error, TEXT("Owner's Ability System Component is Not Valid in ACaptiveSunProjectile::Burst()"));
		return;
	}

	// Get Team Subsystem
	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();
	if (!IsValid(TeamSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("Team Subsystem is Not Valid in ACaptiveSunProjectile::Burst()"));
		return;
	}

	// Direct Damage 
	// Compare Teams (Different Teams)
	if (TeamSubsystem->CompareTeams(OwnerActor, DirectHitActor) == EOWTeamComparison::DifferentTeams)
	{
		// Apply Direct Damage Effect
		// TODO - Literal
		IAbilitySystemInterface* DirectHitActorWithASC = Cast<IAbilitySystemInterface>(DirectHitActor);
		if (OwnerWithASC && DirectHitActorWithASC)
		{
			DamageEffectParams.BaseDamage = 5.f;
			DamageEffectParams.SourceAbilitySystemComponent = OwnerWithASC->GetAbilitySystemComponent();
			DamageEffectParams.TargetAbilitySystemComponent = DirectHitActorWithASC->GetAbilitySystemComponent();
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}

	// Spawn Impact Effect - Explosion 
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GetImpactEffect(), ProjectileLocation);

	TArray<AActor*> OverlappingActors;
	TArray<AActor*> IgnoredActors;
	UOWAbilitySystemLibrary::GetLivePlayersWithinRadius(this, OverlappingActors, IgnoredActors, Radius, ProjectileLocation);

	// Iterate Hit Reults 
	for (AActor* HitActor : OverlappingActors)
	{
		// Get Hit Actor and Cast Ability System Interface 
		IAbilitySystemInterface* HitActorWithASC = Cast<IAbilitySystemInterface>(HitActor);

		// Check if Hit Actor is Valid
		if (!IsValid(HitActor) || !HitActorWithASC)
		{
			continue;
		}

		// Get Ability System Component from Hit Actor
		UAbilitySystemComponent* HitActorASC = HitActorWithASC->GetAbilitySystemComponent();
		if (!IsValid(HitActorASC))
		{
			continue;
		}
		
		// Compare Teams (Different Teams)
		if (TeamSubsystem->CompareTeams(OwnerActor, HitActor) == EOWTeamComparison::DifferentTeams)
		{
			// Apply Damage Effect
			// TODO - Literal
			DamageEffectParams.BaseDamage = 50.f;
			DamageEffectParams.SourceAbilitySystemComponent = OwnerWithASC->GetAbilitySystemComponent();
			DamageEffectParams.TargetAbilitySystemComponent = HitActorWithASC->GetAbilitySystemComponent();
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			//Add 'Laceration' Gameplay Cue On Actor 
			FGameplayCueParameters GameplayCueParameters;
			GameplayCueParameters.Instigator = OwnerActor;
			UGameplayCueFunctionLibrary::AddGameplayCueOnActor(
				HitActor,
				FOWGameplayTags::Get().GameplayCue_Illiari_Laceration,
				GameplayCueParameters);

			// Apply 'Laceration' Gameplay Effect to Actor
			FGameplayEffectContextHandle LacerationEffectContextHandle = OwnerASC->MakeEffectContext();
			LacerationEffectContextHandle.AddInstigator(OwnerActor, this);
			FGameplayEffectSpecHandle LacerationEffectSpecHandle =
				OwnerASC->MakeOutgoingSpec(LacerationGameplayEffectClass, 1.f, LacerationEffectContextHandle);
			OwnerASC->ApplyGameplayEffectSpecToTarget(*LacerationEffectSpecHandle.Data.Get(), HitActorASC);
		}
	}
}

void ACaptiveSunProjectile::LifeSpanExpired()
{
	if (!bHasExploded)
	{
		Burst(GetActorLocation());
	}

	bHasExploded = true;

	Super::LifeSpanExpired();
}
