// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnergyBarrier.h"
#include "Components/BoxComponent.h"
#include "Team/OWTeamSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AEnergyBarrier::AEnergyBarrier()
{
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Box->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	SetRootComponent(Box);
}

void AEnergyBarrier::BeginPlay()
{
	Super::BeginPlay(); 

	if (HasAuthority())
	{
		FGameplayAbilitySpec RegenBarrierFieldAbilitySpec = FGameplayAbilitySpec(RegenerateBarrierFieldAbilityClass, 1 /*int32 InLevel*/);
		GetAbilitySystemComponent()->GiveAbility(RegenBarrierFieldAbilitySpec);

		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			UE_LOG(LogTemp, Warning, TEXT("Energy Barrier Ability: %s"), *Spec.Ability->GetName());
		}
	}
}

void AEnergyBarrier::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	// TODO - Change Literal 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UOWAttributeSet::GetHealthAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				HandleHealth(Data.NewValue);
			});
}

void AEnergyBarrier::HandleHealth(float NewHealthValue)
{
	if (NewHealthValue <= 0.f)
	{
		bBarrierDestroyed = true; 
		// TODO - Physics 
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetAttachParentActor(),
			FOWGameplayTags::Get().Event_Reinhardt_BarrierDestroyed,
			Payload);
	}
	else if (NewHealthValue <= AttributeSet->GetMaxHealth() * 0.25f && !bBarrierVeryDangerous)
	{
		bBarrierVeryDangerous = true; 
		// TODO - Physics 

	}
	else if (NewHealthValue <= AttributeSet->GetMaxHealth() * 0.5f && !bBarrierDangerous)
	{
		bBarrierDangerous = true; 
		// TODO - Physics 
	}
	else
	{
		bBarrierDangerous = false; 
		bBarrierVeryDangerous = false;
		bBarrierDestroyed = true; 
	}
}
