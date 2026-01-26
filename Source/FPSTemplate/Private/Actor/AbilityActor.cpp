// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AbilityActor.h"
#include "Net/UnrealNetwork.h"
#include "Team/OWTeamSubsystem.h"

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

void AAbilityActor::InitializeTeamID()
{
	AActor* AbilityOwner = GetOwner(); 
	if (!IsValid(AbilityOwner))
	{
		return;
	}

	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>(); 
	if (!IsValid(TeamSubsystem))
	{
		return;
	}

	SetTeamID(TeamSubsystem->FindTeamFromObject(AbilityOwner));
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	InitializeTeamID();
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
	if (!IsValid(VitalAttributes))
	{
		return;
	}

	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();

	VitalAttributesContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle VitalAttributeSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(VitalAttributes, Level, VitalAttributesContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}
