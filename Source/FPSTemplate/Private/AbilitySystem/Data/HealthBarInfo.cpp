// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/HealthBarInfo.h"

FBarInfo UHealthBarInfo::FindHealthBarInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FBarInfo& Info : HealthBarInformation)
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

	return FBarInfo();
}
