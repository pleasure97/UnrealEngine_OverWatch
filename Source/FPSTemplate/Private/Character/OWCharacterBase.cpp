// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacterBase.h"


AOWCharacterBase::AOWCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); 
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket")); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void AOWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

