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
	// Get Owner and Cast Ability System Interface 
	AActor* OwnerActor = GetOwner(); 
	IAbilitySystemInterface* OwnerWithASC = Cast<IAbilitySystemInterface>(OwnerActor);

	// Check if Owner Actor and Direct Hit Actor are Valid
	if (!IsValid(OwnerActor) || !OwnerWithASC)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is Not Valid in ACaptiveSunProjectile::Burst()"));
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
	if (TeamSubsystem->CompareTeams(OwnerActor, OtherActor) == EOWTeamComparison::DifferentTeams)
	{
		// Apply Direct Damage Effect
		// TODO - Literal
		IAbilitySystemInterface* DirectHitActorWithASC = Cast<IAbilitySystemInterface>(OtherActor);
		if (OwnerWithASC && DirectHitActorWithASC)
		{
			DamageEffectParams.BaseDamage = 5.f;
			DamageEffectParams.SourceAbilitySystemComponent = OwnerWithASC->GetAbilitySystemComponent();
			DamageEffectParams.TargetAbilitySystemComponent = DirectHitActorWithASC->GetAbilitySystemComponent();
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}

	// Spawn Impact Effect - Explosion 
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GetImpactEffect(), Hit.Location);

	// Trace Multi Sphere 
	TArray<AActor*> IgnoredActors;
	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(this, Hit.Location, Hit.Location, Radius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn), false, IgnoredActors, EDrawDebugTrace::None, HitResults, true);
	if (!bHit)
	{
		return;
	}

	for (const FHitResult& HitResult : HitResults)
	{
		// Get Hit Actor and Cast Ability System Interface 
		AActor* HitActor = HitResult.GetActor();
		IAbilitySystemInterface* HitActorWithASC = Cast<IAbilitySystemInterface>(HitActor); 

		// Check if Hit Actor is Valid
		if (!IsValid(HitActor) || !HitActorWithASC)
		{
			continue;
		}

		// Compare Teams (Different Teams)
		if (TeamSubsystem->CompareTeams(OwnerActor, HitResult.GetActor()) == EOWTeamComparison::DifferentTeams)
		{
			// Apply Damage Effect
			// TODO - Literal
			DamageEffectParams.BaseDamage = 50.f;
			DamageEffectParams.SourceAbilitySystemComponent = OwnerWithASC->GetAbilitySystemComponent();
			DamageEffectParams.TargetAbilitySystemComponent = HitActorWithASC->GetAbilitySystemComponent();
			UOWAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			//Add Gameplay Cue On Actor 
			FGameplayCueParameters GameplayCueParameters;
			UGameplayCueFunctionLibrary::AddGameplayCueOnActor(
				HitActor,
				FOWGameplayTags::Get().GameplayCue_Illiari_Laceration, 
				GameplayCueParameters);
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
