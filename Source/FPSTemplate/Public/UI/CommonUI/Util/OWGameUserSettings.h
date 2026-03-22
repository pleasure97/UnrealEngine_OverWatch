// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "OWGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UOWGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UOWGameUserSettings();

	static UOWGameUserSettings* Get();

	/* Gameplay Collection Tab */
	UFUNCTION()
	FString GetCurrentEnableAutomaticQueueRegistration() const { return CurrentEnableAutomaticQueueRegistration; }

	UFUNCTION()
	void SetCurrentEnableAutomaticQueueRegistration(const FString& InNewEnableAutomaticQueueRegistration) { CurrentEnableAutomaticQueueRegistration = InNewEnableAutomaticQueueRegistration;}

	UFUNCTION()
	FString GetCurrentLimitClientTransmissionRate() const { return CurrentLimitClientTransmissionRate; }

	UFUNCTION()
	void SetCurrentLimitClientTransmissionRate(const FString& InNewLimitClientTransmissionRate) { CurrentLimitClientTransmissionRate = InNewLimitClientTransmissionRate; }
	/* Gameplay Collection Tab Ends */

	/* Audio Collection Tab */
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(float InOverallVolume);

	UFUNCTION()
	float GetSoundEffectVolume() const { return SoundEffectVolume; }

	UFUNCTION()
	void SetSoundEffectVolume(float InSoundEffectVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(float InMusicVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bInAllowBackgroundAudio);

	UFUNCTION()
	bool GetPlayNotificationWhenAllyFalls() const { return bPlayNotificationWhenAllyFalls; }

	UFUNCTION()
	void SetPlayNotificationWhenAllyFalls(bool bInPlayNotificationWhenAllyFalls);

	UFUNCTION()
	bool GetPlayNotificationWhenKillingEnemy() const { return bPlayNotificationWhenKillingEnemy; }

	UFUNCTION()
	void SetPlayNotificationWhenKillingEnemy(bool bInPlayNotificationWhenKillingEnemy);
	/* Audio Collection Tab Ends */
private:
	/* Gameplay Collection Tab */
	UPROPERTY(Config)
	FString CurrentEnableAutomaticQueueRegistration;

	UPROPERTY(Config)
	FString CurrentLimitClientTransmissionRate;
	/* Gameplay Collection Tab Ends */

	/* Audio Collection Tab */
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float SoundEffectVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;

	UPROPERTY(Config)
	bool bPlayNotificationWhenAllyFalls;

	UPROPERTY(Config)
	bool bPlayNotificationWhenKillingEnemy;
	/* Audio Collection Tab Ends */
};
