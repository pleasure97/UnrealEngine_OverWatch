// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IlliariCharacter.h"
#include "AbilitySystem/GameplayAbilityTargetActor/OWGATargetActor_LineTrace.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Support/Illiari/HealingRay.h"

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
	return FTransform();
}

FTransform AIlliariCharacter::GetThirdPersonWeaponSocketTransform_Implementation()
{
	return FTransform();
}

UNiagaraComponent* AIlliariCharacter::GetHealingRayNiagaraComponent() const
{
	return HealingRayNiagaraComponent;
}

void AIlliariCharacter::RegenerateSolarRifle()
{
}

void AIlliariCharacter::RegenerateHealingRay()
{
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				TSubclassOf<UGameplayAbility> HealingRayAbilityClass = UHealingRay::StaticClass();
				if (FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(HealingRayAbilityClass))
				{
					if (UGameplayAbility* PrimaryGameplayAbility = GameplayAbilitySpec->GetPrimaryInstance())
					{
						if (UHealingRay* HealingRayAbility = Cast<UHealingRay>(PrimaryGameplayAbility))
						{
							if (HealingRayAbility->IsActive())
							{
								World->GetTimerManager().ClearTimer(HealingRayRegenTimerHandle); 
							}
							bool bFullCharged = HealingRayAbility->RegenerateHealGaugeByTick();
							if (bFullCharged)
							{
								World->GetTimerManager().ClearTimer(HealingRayRegenTimerHandle);
							}
						}
					}
				}
			}
		}
	}
}

void AIlliariCharacter::SetHealingRayRegenTimerHandle(FTimerHandle InHealingRayRegenTimerHandle)
{
	HealingRayRegenTimerHandle = InHealingRayRegenTimerHandle; 
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

	// Spawn System Attached - Healing Ray 
	HealingRayNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		HealingRayNiagaraSystem, FirstPersonWeapon, FName("BeamSocket"),
		FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset, false, false);

	/*HealingRayNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		HealingRayNiagaraSystem, ThirdPersonWeapon, FName("BeamSocket"),
		FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset, false, false);*/
}


