// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/OWInputConfig.h"

const UInputAction* UOWInputConfig::FindAbilityInputForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FOWInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction; 
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this)); 
	}

	return nullptr; 
}
