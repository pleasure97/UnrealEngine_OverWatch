// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacterBase.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"


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

