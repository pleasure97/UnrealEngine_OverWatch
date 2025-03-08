// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWOmnic.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

AOWOmnic::AOWOmnic()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); 

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 
}

int32 AOWOmnic::GetCharacterLevel_Implementation() const
{
	return Level; 
}

void AOWOmnic::Die(const FVector& DeathImpulse)
{
}

EOmnicClass AOWOmnic::GetOmnicClass_Implementation()
{
	return OmnicClass;
}

void AOWOmnic::BeginPlay()
{
	Super::BeginPlay(); 

	AbilitySystemComponent->InitAbilityActorInfo(this, this); 
}
