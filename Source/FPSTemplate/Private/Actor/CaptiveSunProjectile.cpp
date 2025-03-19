// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CaptiveSunProjectile.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void ACaptiveSunProjectile::Shoot()
{
	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); 

	if (GetOwner()->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* SolarRifle = ICombatInterface::Execute_GetWeapon(GetOwner()); 
		const FTransform CaptiveSunTransform = SolarRifle->GetSocketTransform("BeamSocket", ERelativeTransformSpace::RTS_ParentBoneSpace);
		const FRotator& CaptiveSunRotation = CaptiveSunTransform.Rotator(); 
		FVector NewVelocity = UKismetMathLibrary::GetForwardVector(CaptiveSunRotation) * ProjectileVelocity; 
		ProjectileMovement->SetVelocityInLocalSpace(NewVelocity); 
		ProjectileMovement->Activate(); 

		SetActorEnableCollision(true); 
		Sphere->OnComponentHit.AddDynamic(this, &ACaptiveSunProjectile::Burst); 
	}
}

void ACaptiveSunProjectile::Burst(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Spawn Impact Effect - Explosion 
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GetImpactEffect(), Hit.Location, FRotator::ZeroRotator, FVector(0.8f, 0.8f, 0.8f)); 

	// TODO - Change Trace Channel to trace enemy 
	TArray<AActor*> IgnoredActors; 
	TArray<FHitResult> HitResults; 
	bool bEnemiesHit = UKismetSystemLibrary::SphereTraceMulti(this, Hit.Location, Hit.Location, Radius, 
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, HitResults, true); 
	if (!bEnemiesHit) return; 

	for (const FHitResult& HitResult : HitResults)
	{

	}

}
