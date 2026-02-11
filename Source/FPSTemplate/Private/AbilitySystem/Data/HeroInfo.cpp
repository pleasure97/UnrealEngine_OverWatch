// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/HeroInfo.h"

TArray<EHeroName> UHeroInfo::GetAllHeroNames() const
{
    TArray<EHeroName> HeroNames;

    HeroInformation.GetKeys(HeroNames);

    return HeroNames;
}

TArray<EHeroClass> UHeroInfo::GetAllHeroClasses() const
{
    TArray<EHeroClass> Keys;
    CommonClassInformation.GenerateKeyArray(Keys);
    return Keys;
}

UAnimMontage* UHeroInfo::GetAnimMontageForTag(const EHeroName& HeroName, const FGameplayTag& AbilityTag) const
{
    for (const TPair<FGameplayTag, UAnimMontage*> AnimationMontagePair : HeroInformation[HeroName].AnimationMontages)
    {
        if (AnimationMontagePair.Key.MatchesTagExact(AbilityTag))
        {
            return AnimationMontagePair.Value;
        }
    }
    return nullptr;
}

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
