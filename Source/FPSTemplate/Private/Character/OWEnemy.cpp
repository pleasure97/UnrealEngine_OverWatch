// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWEnemy.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

AOWEnemy::AOWEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); 

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 
}
