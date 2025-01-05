// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWEnemy.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

AOWEnemy::AOWEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 
}
