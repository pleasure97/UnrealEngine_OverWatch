// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnergyBarrier.h"
#include "Components/BoxComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

AEnergyBarrier::AEnergyBarrier()
{
	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Box->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	SetRootComponent(Box);

	BarrierField = CreateDefaultSubobject<UStaticMeshComponent>("BarrierField");
	BarrierField->SetupAttachment(GetRootComponent());

	AbilitySystemComponent = CreateDefaultSubobject<UOWAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UOWAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AEnergyBarrier::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent; 
}

void AEnergyBarrier::BeginPlay()
{
	Super::BeginPlay(); 

	InitAbilityActorInfo(); 
}

void AEnergyBarrier::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UOWAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeVitalAttributes();
	}

}

void AEnergyBarrier::InitializeVitalAttributes()
{
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();

	VitalAttributesContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle VitalAttributeSpecHandle =
		AbilitySystemComponent->MakeOutgoingSpec(VitalAttributes, Level, VitalAttributesContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}
