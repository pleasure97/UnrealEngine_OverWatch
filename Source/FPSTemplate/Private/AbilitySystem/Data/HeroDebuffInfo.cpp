// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/HeroDebuffInfo.h"

FHeroDebuffUIInfo* UHeroDebuffInfo::GetHeroDebuffUIInfo(const FGameplayTag& DebuffTag)
{
	if (FHeroDebuffUIInfo* HeroDebuffUIInfo = HeroDebuffInfoMap.Find(DebuffTag))
	{
		return HeroDebuffUIInfo;
	}
	return nullptr;
}
