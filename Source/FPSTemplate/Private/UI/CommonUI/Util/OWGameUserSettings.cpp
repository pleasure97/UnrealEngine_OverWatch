// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Util/OWGameUserSettings.h"

UOWGameUserSettings::UOWGameUserSettings()
	: OverallVolume(100.f), SoundEffectVolume(100.f), MusicVolume(100.f), bAllowBackgroundAudio(true)
{
}

UOWGameUserSettings* UOWGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UOWGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UOWGameUserSettings::SetOverallVolume(float InOverallVolume)
{
	OverallVolume = InOverallVolume;

	// Actual Logic for Controlling Volume Goes Here

}

void UOWGameUserSettings::SetSoundEffectVolume(float InSoundEffectVolume)
{
	SoundEffectVolume = InSoundEffectVolume;

	// Actual Logic for Controlling Volume Goes Here
}

void UOWGameUserSettings::SetMusicVolume(float InMusicVolume)
{
	MusicVolume = InMusicVolume;

	// Actual Logic for Controlling Volume Goes Here
}

void UOWGameUserSettings::SetAllowBackgroundAudio(bool bInAllowBackgroundAudio)
{
	bAllowBackgroundAudio = bInAllowBackgroundAudio;

	// Actual Logic for Controlling Audio Goes Here
}

void UOWGameUserSettings::SetPlayNotificationWhenAllyFalls(bool bInPlayNotificationWhenAllyFalls)
{
	bPlayNotificationWhenAllyFalls = bInPlayNotificationWhenAllyFalls;
}

void UOWGameUserSettings::SetPlayNotificationWhenKillingEnemy(bool bInPlayNotificationWhenKillingEnemy)
{
	bPlayNotificationWhenKillingEnemy = bInPlayNotificationWhenKillingEnemy;
}



