// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DefensiveAttributeInfo.h"

FAttributeDefensiveInfo UDefensiveAttributeInfo::FindDefensiveAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAttributeDefensiveInfo& Info : DefensiveAttributeInformation)
	{
		if (Info.DefensiveAttributeTag == AttributeTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for Ability Tag [%s] on AbilityInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAttributeDefensiveInfo();
}
