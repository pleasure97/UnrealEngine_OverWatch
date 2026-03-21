// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Util/OWGameUserSettings.h"

UOWGameUserSettings* UOWGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UOWGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}



