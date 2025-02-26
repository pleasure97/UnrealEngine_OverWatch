// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/HeroInfo.h"

FOWAbilityInfo UHeroInfo::FindAbilityInfoForTag(const EHeroName& HeroName, const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
    const FOWHeroInfo& OWHeroInfo = HeroInformation[HeroName]; 

    const TArray<FOWAbilityInfo>& AbilityInfos = OWHeroInfo.Abilities;

    for (const FOWAbilityInfo& AbilityInfo : AbilityInfos)
    {
        if (AbilityInfo.AbilityTag == AbilityTag) { return AbilityInfo; }
    }

   if (bLogNotFound)
   { 
       UE_LOG(LogTemp, Error, TEXT("Can't find info for Ability Tag [%s] on HeroInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this)); 
   }

   return FOWAbilityInfo(); 
}
