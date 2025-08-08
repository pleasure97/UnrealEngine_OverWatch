// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IlliariCharacter.h"

AIlliariCharacter::AIlliariCharacter()
{
	HealingPylon = CreateDefaultSubobject<UChildActorComponent>("HealingPylon"); 
	HealingPylon->SetupAttachment(GetRootComponent());
}

FVector AIlliariCharacter::GetProjectileStartLocation_Implementation() const
{
	return HealingPylon->GetComponentLocation();
}



