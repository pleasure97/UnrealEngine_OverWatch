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

void AOWOmnic::BeginPlay()
{
	Super::BeginPlay(); 

	AbilitySystemComponent->InitAbilityActorInfo(this, this); 
}
