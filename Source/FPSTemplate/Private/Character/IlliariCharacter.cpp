// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IlliariCharacter.h"
#include "AbilitySystem/GameplayAbilityTargetActor/OWGATargetActor_LineTrace.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Support/Illiari/HealingRay.h"
#include "NiagaraComponent.h"

AIlliariCharacter::AIlliariCharacter()
{
	// Healing Pylon 
	HealingPylon = CreateDefaultSubobject<UChildActorComponent>("HealingPylon");
	HealingPylon->SetupAttachment(GetRootComponent());

	// First Person Weapon
	FirstPersonWeapon = CreateDefaultSubobject<UStaticMeshComponent>("FirstPersonWeapon");
	FirstPersonWeapon->SetupAttachment(FirstPersonMesh);
	FTransform FirstPersonWeaponTransform;
	FirstPersonWeaponTransform.SetLocation(FVector(-12.6f, 3.9f, 120.9f));
	FirstPersonWeaponTransform.SetRotation(FQuat::MakeFromRotator(FRotator(0.f, 90.f, 0.f)));
	FirstPersonWeaponTransform.SetScale3D(FVector(0.09f, 0.09f, 0.09f));
	FirstPersonWeapon->SetRelativeTransform(FirstPersonWeaponTransform);

	// Third Person Weapon 
	ThirdPersonWeapon = CreateDefaultSubobject<UStaticMeshComponent>("ThirdPersonWeapon");
	ThirdPersonWeapon->SetupAttachment(GetMesh());
	FTransform ThirdPersonWeaponTransform;
	ThirdPersonWeaponTransform.SetLocation(FVector(-24.f, 38.f, 139.f));
	ThirdPersonWeaponTransform.SetRotation(FQuat::MakeFromRotator(FRotator(0.f, 90.f, 0.f)));
	ThirdPersonWeaponTransform.SetScale3D(FVector(0.09f, 0.09f, 0.09f));
	ThirdPersonWeapon->SetRelativeTransform(ThirdPersonWeaponTransform);

	// First Person Healing Ray Niagara Component
	FirstPersonHealingRay = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FirstPersonHealingRay"));
	FirstPersonHealingRay->SetupAttachment(FirstPersonWeapon, FName("BeamSocket"));
	FirstPersonHealingRay->bAutoActivate = false;
	FirstPersonHealingRay->SetOnlyOwnerSee(true);

	// Third Person Healing Ray Niagara Component
	ThirdPersonHealingRay = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ThirdPersonHealingRay"));
	ThirdPersonHealingRay->SetupAttachment(ThirdPersonWeapon, FName("BeamSocket"));
	ThirdPersonHealingRay->bAutoActivate = false;
	ThirdPersonHealingRay->SetOwnerNoSee(true);
}

AOWGATargetActor_LineTrace* AIlliariCharacter::GetLineTraceTargetActor_Implementation()
{
	if (LineTraceTargetActor)
	{
		return LineTraceTargetActor;
	}

	LineTraceTargetActor = GetWorld()->SpawnActor<AOWGATargetActor_LineTrace>();
	LineTraceTargetActor->SetOwner(this);

	return LineTraceTargetActor;
}

FVector AIlliariCharacter::GetProjectileStartLocation_Implementation() const
{
	return HealingPylon->GetComponentLocation();
}

FTransform AIlliariCharacter::GetFirstPersonWeaponSocketTransform_Implementation()
{
	return FirstPersonWeapon->GetSocketTransform(FName("BeamSocket"));
}

FTransform AIlliariCharacter::GetThirdPersonWeaponSocketTransform_Implementation()
{
	return ThirdPersonWeapon->GetSocketTransform(FName("BeamSocket"));
}

UNiagaraComponent* AIlliariCharacter::GetHealingRayNiagaraComponent() const
{
	return IsLocallyControlled() ? FirstPersonHealingRay : ThirdPersonHealingRay;
}

UStaticMeshComponent* AIlliariCharacter::GetFirstPersonWeapon() const
{
	return FirstPersonWeapon; 
}

UStaticMeshComponent* AIlliariCharacter::GetThirdPersonWeapon() const
{
	return ThirdPersonWeapon; 
}

void AIlliariCharacter::BeginPlay()
{
	Super::BeginPlay(); 

	if (IsValid(FirstPersonHealingRay))
	{
		FirstPersonHealingRay->SetVectorParameter(FName("BeamStart"), FirstPersonWeapon->GetSocketLocation(FName("BeamSocket")));
	}
	if (IsValid(ThirdPersonHealingRay))
	{
		ThirdPersonHealingRay->SetVectorParameter(FName("BeamStart"), ThirdPersonWeapon->GetSocketLocation(FName("BeamSocket")));
	}
}

