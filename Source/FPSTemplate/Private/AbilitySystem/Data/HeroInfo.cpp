// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/HeroInfo.h"

FOWHeroInfo UHeroInfo::GetHeroDefaultInfo(EHeroName HeroName)
{
    return HeroInformation.FindChecked(HeroName); 
}
