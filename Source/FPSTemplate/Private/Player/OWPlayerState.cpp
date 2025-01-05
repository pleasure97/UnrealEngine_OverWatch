// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

AOWPlayerState::AOWPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 

	NetUpdateFrequency = 100.f; 
}

UAbilitySystemComponent* AOWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
