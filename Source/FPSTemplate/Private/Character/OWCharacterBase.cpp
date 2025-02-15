// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacterBase.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"


AOWCharacterBase::AOWCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); 
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket")); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

UAbilitySystemComponent* AOWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AOWCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage; 
}

FOnASCRegistered& AOWCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered; 
}

FOnDeath& AOWCharacterBase::GetOnDeathDelegate()
{
	return OnDeath; 
}

void AOWCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); 
	MulticastHandleDeath(DeathImpulse); 
}

bool AOWCharacterBase::IsDead_Implementation() const
{
	return bDead; 
}

void AOWCharacterBase::MulticastHandleDeath(const FVector& DeathImpulse)
{
	Weapon->SetSimulatePhysics(true); 
	Weapon->SetEnableGravity(true); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true); 

	GetMesh()->SetEnableGravity(true); 
	GetMesh()->SetSimulatePhysics(true); 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true); 

	bDead = true; 
	OnDeath.Broadcast(this); 
}

void AOWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOWCharacterBase::AddHeroAbilities()
{
	UOWAbilitySystemComponent* OWASC = CastChecked<UOWAbilitySystemComponent>(AbilitySystemComponent); 

	if (!HasAuthority()) return; 

	OWASC->AddHeroAbilities(DefaultAbilities); 
}

