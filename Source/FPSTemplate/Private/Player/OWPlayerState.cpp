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

	MyTeamID = FGenericTeamId::NoTeam;
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
	DOREPLIFETIME(AOWPlayerState, MyTeamID); 
	DOREPLIFETIME(AOWPlayerState, SelectedHeroName);
	DOREPLIFETIME(AOWPlayerState, Level); 
	DOREPLIFETIME(AOWPlayerState, XP); 
	DOREPLIFETIME(AOWPlayerState, AttributePoints); 
	DOREPLIFETIME(AOWPlayerState, SpellPoints); 
}

void AOWPlayerState::AddToXP(int32 InXP)
{
	XP += InXP; 
	OnXPChangedDelegate.Broadcast(XP); 
}

void AOWPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel; 
	OnLevelChangedDelegate.Broadcast(Level, true); 
}

void AOWPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints; 
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints); 
}

void AOWPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints; 
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints); 
}

void AOWPlayerState::SetXP(int32 InXP)
{
	XP = InXP; 
	OnXPChangedDelegate.Broadcast(XP); 
}

void AOWPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel; 
	OnLevelChangedDelegate.Broadcast(Level, false); 
}

void AOWPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints); 
}

void AOWPlayerState::SetSpellPoints(int32 InSpellPoints)
{
	SpellPoints = InSpellPoints; 
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints); 
}

void AOWPlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID; 
		MARK_PROPERTY_DIRTY_FROM_NAME(AOWPlayerState, MyTeamID, this);
		MyTeamID = NewTeamID; 
		BroadcastTeamChanged(this, OldTeamID, NewTeamID); 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this)); 
	}
}

FGenericTeamId AOWPlayerState::GetGenericTeamId() const
{
	return MyTeamID; 
}

FOnTeamIndexChangedDelegate* AOWPlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate; 
}


void AOWPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true); 
}

void AOWPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP); 
}

void AOWPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints); 
}

void AOWPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints); 
}

void AOWPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	BroadcastTeamChanged(this, OldTeamID, MyTeamID); 
}

