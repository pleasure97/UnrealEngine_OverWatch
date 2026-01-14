// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AbilityActor.h"
#include "Net/UnrealNetwork.h"

AAbilityActor::AAbilityActor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet");
}

UOWAttributeSet* AAbilityActor::GetAttributeSet() const
{
	return AttributeSet;
}

UAbilitySystemComponent* AAbilityActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGenericTeamId AAbilityActor::GetGenericTeamId() const
{
	return IntegerToGenericTeamId(TeamID);
}

void AAbilityActor::SetTeamID(const int32 InTeamID)
{
	TeamID = InTeamID;
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbilityActor, TeamID);
}

void AAbilityActor::InitAbilityActorInfo()
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

	}

	if (HasAuthority())
	{
		InitializeVitalAttributes();
	}
}

void AAbilityActor::InitializeVitalAttributes()
{
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();

	VitalAttributesContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle VitalAttributeSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(VitalAttributes, Level, VitalAttributesContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}
