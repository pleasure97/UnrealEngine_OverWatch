// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "AbilitySystem/OWGlobalAbilitySystem.h"
#include "Net/UnrealNetwork.h"

AOWPlayerState::AOWPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent"); 
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet"); 

	NetUpdateFrequency = 100.f; 

	MyTeamID = FGenericTeamId::NoTeam;
}

UAbilitySystemComponent* AOWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AOWPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AOWPlayerState::SetHeroName(EHeroName NewHeroName)
{
	HeroName = NewHeroName;
	OnRep_HeroName();
}

void AOWPlayerState::OnRep_HeroName()
{
	FString EnumName = UEnum::GetValueAsString(HeroName);
	UE_LOG(LogTemp, Log, TEXT("Hero Changed to %s"), *EnumName);
	OnHeroNameChangedDelegate.Broadcast(this, HeroName);
}

void AOWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	// Apply Pushed Model to Player State 
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	// Use Property ID Internally to Find Properties Faster
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, HeroName, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, Level, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, XP, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, AttributePoints, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, SpellPoints, SharedParams);

	// Skip Owner - Replicated View Rotation
	SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(AOWPlayerState, ReplicatedViewRotation, SharedParams);
}

void AOWPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AbilitySystemComponent)
	{
		if (UOWGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UOWGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(AbilitySystemComponent);
		}
	}
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

FRotator AOWPlayerState::GetReplicatedViewRotation() const
{
	return ReplicatedViewRotation;
}

void AOWPlayerState::SetReplicatedViewRotation(const FRotator& NewRotation)
{
	if (NewRotation != ReplicatedViewRotation)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(AOWPlayerState, ReplicatedViewRotation, this);
		ReplicatedViewRotation = NewRotation;
	}
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