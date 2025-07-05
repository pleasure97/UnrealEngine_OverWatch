// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/WeaponReload.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

void UWeaponReload::ReloadBullets()
{
	check(CurrentActorInfo); 

	if (CurrentActorInfo->AbilitySystemComponent.IsValid())
	{
		if (UOWAttributeSet* OWAttributeSet = const_cast<UOWAttributeSet*>(CurrentActorInfo->AbilitySystemComponent->GetSet<UOWAttributeSet>()))
		{
			float NumMaxBullets = OWAttributeSet->GetNumMaxBullets(); 
			OWAttributeSet->SetNumCurrentBullets(NumMaxBullets);
		}
	}
}
