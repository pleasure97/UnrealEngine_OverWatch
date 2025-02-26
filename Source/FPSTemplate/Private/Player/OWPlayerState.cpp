// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Net/UnrealNetwork.h"

AOWPlayerState::AOWPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 

	SetSelectedHeroName(EHeroName::ILLIARI); 

	NetUpdateFrequency = 100.f; 
}

UAbilitySystemComponent* AOWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOWPlayerState::SetSelectedHeroName(EHeroName NewHeroName)
{
	SelectedHeroName = NewHeroName;
	OnRep_SelectedHeroName();
}

void AOWPlayerState::OnRep_SelectedHeroName()
{
	UE_LOG(LogTemp, Log, TEXT("Hero changed!"));
}

void AOWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(AOWPlayerState, SelectedHeroName);
}
