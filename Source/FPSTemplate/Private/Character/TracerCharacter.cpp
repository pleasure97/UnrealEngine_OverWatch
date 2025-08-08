// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TracerCharacter.h"

ATracerCharacter::ATracerCharacter()
{
	PulseBomb = CreateDefaultSubobject<UChildActorComponent>("PulseBomb"); 
	PulseBomb->SetupAttachment(GetRootComponent()); 
}

FVector ATracerCharacter::GetProjectileStartLocation_Implementation() const
{
	return PulseBomb->GetComponentLocation(); 
}
