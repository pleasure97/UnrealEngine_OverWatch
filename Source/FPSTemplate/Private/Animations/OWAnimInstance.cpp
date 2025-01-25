// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/OWAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Character/OWCharacter.h"

UOWAnimInstance::UOWAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOWAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC); 

	GameplayTagPropertyMap.Initialize(this, ASC); 
}

void UOWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); 

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC); 
		}
	}
}
