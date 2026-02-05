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

void ACaptiveSunProjectile::Shoot()
{
	AOWCharacter* OWCharacter = Cast<AOWCharacter>(GetOwner()); 
	if (!IsValid(OWCharacter))
	{
		return;
	}

	APlayerController* OWPlayerController = Cast<APlayerController>(OWCharacter->GetController());
	if (!IsValid(OWPlayerController))
	{
		return;
	}
	
	FVector CameraLocation;
	FRotator CameraRotation;
	OWPlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * ProjectileVelocity * ProjectileLifeSpan);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OWCharacter);
	Params.AddIgnoredActor(this);

	FVector TargetPoint;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		TargetPoint = HitResult.ImpactPoint;
	}
	else
	{
		TargetPoint = TraceEnd;
	}

	FVector LaunchDirection = (TargetPoint - GetActorLocation()).GetSafeNormal();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	SetActorRotation(CameraRotation);

	SetReplicateMovement(true);

	ProjectileMovement->Velocity = LaunchDirection * ProjectileVelocity;
	ProjectileMovement->Activate();

	SetActorEnableCollision(true);
	Sphere->OnComponentHit.AddDynamic(this, &ACaptiveSunProjectile::Burst);
}

void ACaptiveSunProjectile::Burst(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Spawn Impact Effect - Explosion 
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GetImpactEffect(), Hit.Location, FRotator::ZeroRotator, FVector(0.8f, 0.8f, 0.8f));

	// TODO - Change Trace Channel to trace enemy 
	TArray<AActor*> IgnoredActors;
	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(this, Hit.Location, Hit.Location, Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, HitResults, true);
	if (!bHit) return;

	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();

	for (const FHitResult& HitResult : HitResults)
	{
		if (TeamSubsystem->CompareTeams(GetOwner(), HitResult.GetActor()) == EOWTeamComparison::DifferentTeams)
		{
			// Apply Damage Effect
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			/*const FGameplayCueParameters TargetCueParameters =
				UAbilitySystemBlueprintLibrary::MakeGameplayCueParameters()*/

				// Add Gameplay Cue On Actor 
				//UGameplayCueFunctionLibrary::AddGameplayCueOnActor(FOWGameplayTags::Get().GameplayCue_Illiari_Laceration, T

		}
	}
}

void ACaptiveSunProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!OtherActor || OtherActor == this || OverlappedActors.Contains(OtherActor) || OtherActor == GetOwner())
	{
		return;
	}

	OverlappedActors.Add(OtherActor);

	OnHit(OtherActor);
}
