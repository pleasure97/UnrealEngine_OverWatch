// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/OptionDataRegistry.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Options/ListDataObjectScalar.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"

#define LOCTEXT_NAMESPACE "OptionsUI"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UOWGameUserSettings, SetterOrGetterFuncName))

void UOptionDataRegistry::InitOptionDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGraphicCollectionTab();
	InitAudioCollectionTab();
	InitGameplayCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObjectBase*> UOptionDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObjectCollection* const* FoundTabCollectionPointer = RegisteredOptionTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObjectCollection* AvailableTabCollection) -> bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPointer, TEXT("No Valid Tab Found Under ID %s"), *InSelectedTabID.ToString());

	UListDataObjectCollection* FoundTabCollection = *FoundTabCollectionPointer;

	TArray< UListDataObjectBase*> AllChildListItems; 

	for (UListDataObjectBase* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionDataRegistry::FindChildListDataRecursively(UListDataObjectBase* InParentData, TArray<UListDataObjectBase*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObjectBase* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);
		
		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionDataRegistry::InitGraphicCollectionTab()
{
	UListDataObjectCollection* GraphicTabCollection = NewObject<UListDataObjectCollection>(); 
	GraphicTabCollection->SetDataID(FName("GraphicTabCollection")); 
	GraphicTabCollection->SetDataDisplayName(LOCTEXT("Graphic", "Graphic"));

	// Display Category
	{
		UListDataObjectCollection* DisplayCategoryCollection = NewObject<UListDataObjectCollection>(); 
		DisplayCategoryCollection->SetDataID(FName("DisplayTabCollection")); 
		DisplayCategoryCollection->SetDataDisplayName(LOCTEXT("Display", "Display"));

		GraphicTabCollection->AddChildListData(DisplayCategoryCollection); 

		// Display Mode
		{
			UListDataObjectStringEnum* DisplayMode = NewObject<UListDataObjectStringEnum>(); 
			DisplayMode->SetDataID(FName("DisplayMode")); 
			DisplayMode->SetDataDisplayName(LOCTEXT("DisplayMode", "Display Mode")); 
			DisplayMode->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("FullScreen", "Full Screen"));
			DisplayMode->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("BorderlessWindow", "Borderless Window"));
			DisplayMode->AddEnumOption(EWindowMode::Windowed, LOCTEXT("Windowed", "Windowed"));
			
			DisplayMode->SetDefaultValueFromEnumOption(EWindowMode::Fullscreen);

			DisplayMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			DisplayMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));

			DisplayMode->SetShouldApplySettingsImmediately(true);

			DisplayCategoryCollection->AddChildListData(DisplayMode);
		}
	}

	RegisteredOptionTabCollections.Add(GraphicTabCollection);
}

void UOptionDataRegistry::InitAudioCollectionTab()
{
	UListDataObjectCollection* AudioTabCollection = NewObject<UListDataObjectCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(LOCTEXT("Audio", "Audio"));

	// Volume Category
	{
		UListDataObjectCollection* VolumeCategoryCollection = NewObject<UListDataObjectCollection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(LOCTEXT("Volume", "Volume"));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Overall Volume
		{
			UListDataObjectScalar* OverallVolume = NewObject<UListDataObjectScalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(LOCTEXT("Overall Volume", "Overall Volume"));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 100.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 100.f));
			OverallVolume->SetSliderStepSize(1.f);
			OverallVolume->SetDefaultValueFromString(LexToString(100.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Number);
			OverallVolume->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		// Sound Effect Volume
		{
			UListDataObjectScalar* SoundEffectVolume = NewObject<UListDataObjectScalar>();
			SoundEffectVolume->SetDataID(FName("SoundEffectVolume"));
			SoundEffectVolume->SetDataDisplayName(LOCTEXT("Sound Effect Volume", "Sound Effect Volume"));
			SoundEffectVolume->SetDisplayValueRange(TRange<float>(0.f, 100.f));
			SoundEffectVolume->SetOutputValueRange(TRange<float>(0.f, 100.f));
			SoundEffectVolume->SetSliderStepSize(1.f);
			SoundEffectVolume->SetDefaultValueFromString(LexToString(100.f));
			SoundEffectVolume->SetDisplayNumericType(ECommonNumericType::Number);
			SoundEffectVolume->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			SoundEffectVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundEffectVolume));
			SoundEffectVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundEffectVolume));
			SoundEffectVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(SoundEffectVolume);
		}

		// Music Volume
		{
			UListDataObjectScalar* MusicVolume = NewObject<UListDataObjectScalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(LOCTEXT("Music Volume", "Music Volume"));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 100.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 100.f));
			MusicVolume->SetSliderStepSize(1.f);
			MusicVolume->SetDefaultValueFromString(LexToString(100.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Number);
			MusicVolume->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}
	}

	// Device Category
	{
		UListDataObjectCollection* DeviceCategoryCollection = NewObject<UListDataObjectCollection>();
		DeviceCategoryCollection->SetDataID(FName("DeviceCategoryCollection"));
		DeviceCategoryCollection->SetDataDisplayName(LOCTEXT("Device", "Device"));

		AudioTabCollection->AddChildListData(DeviceCategoryCollection);

		// Playback Device
		{
			UListDataObjectStringEnum* PlaybackDevice = NewObject<UListDataObjectStringEnum>();
			PlaybackDevice->SetDataID(FName("PlaybackDevice"));
			PlaybackDevice->SetDataDisplayName(LOCTEXT("PlaybackDevice", "Playback Device"));
			// TODO - Enum 
			PlaybackDevice->AddEnumOption(EPlaybackDeviceType::Default, LOCTEXT("Playback Device Default", "Default"));
			PlaybackDevice->SetDefaultValueFromEnumOption(EPlaybackDeviceType::Default);
			PlaybackDevice->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPlaybackDeviceType));
			PlaybackDevice->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPlaybackDeviceType));
			PlaybackDevice->SetShouldApplySettingsImmediately(true);
			DeviceCategoryCollection->AddChildListData(PlaybackDevice);
		}
		// Spatial Audio
		{
			UListDataObjectStringEnum* SpatialAudio = NewObject<UListDataObjectStringEnum>();
			SpatialAudio->SetDataID(FName("SpatialAudio"));
			SpatialAudio->SetDataDisplayName(LOCTEXT("SpatialAudio", "Spatial Audio"));
			// TODO - Enum 
			SpatialAudio->AddEnumOption(ESpatialAudioType::SystemSettings, LOCTEXT("SystemSettings", "System Settings"));
			SpatialAudio->AddEnumOption(ESpatialAudioType::DolbyAtmosForHeadphones, LOCTEXT("DolbyAtmosForHeadphones", "Dolby Atmos for Headphones"));
			SpatialAudio->SetDefaultValueFromEnumOption(ESpatialAudioType::SystemSettings);
			SpatialAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSpatialAudioType));
			SpatialAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSpatialAudioType));
			SpatialAudio->SetShouldApplySettingsImmediately(true);
			DeviceCategoryCollection->AddChildListData(SpatialAudio);
		}
		// Audio Mix
		{
			UListDataObjectStringEnum* AudioMix = NewObject<UListDataObjectStringEnum>();
			AudioMix->SetDataID(FName("AudioMix"));
			AudioMix->SetDataDisplayName(LOCTEXT("AudioMix", "Audio Mix"));
			AudioMix->AddEnumOption(EAudioMixType::Default, LOCTEXT("Audio Mix Default", "Default"));
			AudioMix->AddEnumOption(EAudioMixType::Speakers, LOCTEXT("Speakers", "Speakers"));
			AudioMix->AddEnumOption(EAudioMixType::Headphones, LOCTEXT("Headphones", "Headphones"));
			AudioMix->AddEnumOption(EAudioMixType::NightMode, LOCTEXT("NightMode", "Night Mode"));
			AudioMix->AddEnumOption(EAudioMixType::StudioReference, LOCTEXT("StudioReference", "Studio Reference"));
			AudioMix->SetDefaultValueFromEnumOption(EAudioMixType::Default);
			AudioMix->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAudioMixType));
			AudioMix->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAudioMixType));
			AudioMix->SetShouldApplySettingsImmediately(true);
			DeviceCategoryCollection->AddChildListData(AudioMix);
		}
	}

	// Sound Category
	{
		UListDataObjectCollection* SoundCategoryCollection = NewObject<UListDataObjectCollection>(); 
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection")); 
		SoundCategoryCollection->SetDataDisplayName(LOCTEXT("Sound", "Sound"));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		// Allow Background Audio
		{
			UListDataObjectStringBool* AllowBackgroundAudio = NewObject<UListDataObjectStringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(LOCTEXT("AllowBackgroundAudio", "Allow Background Audio"));
			AllowBackgroundAudio->OverrideTrueDisplayText(LOCTEXT("Enabled", "Enabled"));
			AllowBackgroundAudio->OverrideFalseDisplayText(LOCTEXT("Disabled", "Disabled"));
			AllowBackgroundAudio->SetTrueAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);
			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		// Play Notification When Ally Falls
		{
			UListDataObjectStringBool* PlayNotificationWhenAllyFalls = NewObject<UListDataObjectStringBool>();
			PlayNotificationWhenAllyFalls->SetDataID(FName("PlayNotificationWhenAllyFalls"));
			PlayNotificationWhenAllyFalls->SetDataDisplayName(LOCTEXT("PlayNotificationWhenAllyFalls", "Play Notification When Ally Falls"));
			PlayNotificationWhenAllyFalls->OverrideTrueDisplayText(LOCTEXT("Enabled", "Enabled"));
			PlayNotificationWhenAllyFalls->OverrideFalseDisplayText(LOCTEXT("Disabled", "Disabled"));
			PlayNotificationWhenAllyFalls->SetTrueAsDefaultValue();
			PlayNotificationWhenAllyFalls->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPlayNotificationWhenAllyFalls));
			PlayNotificationWhenAllyFalls->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPlayNotificationWhenAllyFalls));
			PlayNotificationWhenAllyFalls->SetShouldApplySettingsImmediately(true);
			SoundCategoryCollection->AddChildListData(PlayNotificationWhenAllyFalls);
		}

		// Play Notification When Killing Enemy
		{
			UListDataObjectStringBool* PlayNotificationWhenKillingEnemy = NewObject<UListDataObjectStringBool>();
			PlayNotificationWhenKillingEnemy->SetDataID(FName("PlayNotificationWhenKillingEnemy"));
			PlayNotificationWhenKillingEnemy->SetDataDisplayName(LOCTEXT("PlayNotificationWhenKillingEnemy", "Play Notification When Killing Enemy"));
			PlayNotificationWhenKillingEnemy->OverrideTrueDisplayText(LOCTEXT("Enabled", "Enabled"));
			PlayNotificationWhenKillingEnemy->OverrideFalseDisplayText(LOCTEXT("Disabled", "Disabled"));
			PlayNotificationWhenKillingEnemy->SetTrueAsDefaultValue();
			PlayNotificationWhenKillingEnemy->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPlayNotificationWhenKillingEnemy));
			PlayNotificationWhenKillingEnemy->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPlayNotificationWhenKillingEnemy));
			PlayNotificationWhenKillingEnemy->SetShouldApplySettingsImmediately(true);
			SoundCategoryCollection->AddChildListData(PlayNotificationWhenKillingEnemy);
		}
	}

	RegisteredOptionTabCollections.Add(AudioTabCollection);
}

void UOptionDataRegistry::InitGameplayCollectionTab()
{
	UListDataObjectCollection* GameplayTabCollection = NewObject<UListDataObjectCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(LOCTEXT("Gameplay", "Gameplay"));

	// Enable Automatic Queue Registration
	{
		UListDataObjectString* EnableAutomaticQueueRegistration = NewObject<UListDataObjectString>();
		EnableAutomaticQueueRegistration->SetDataID(FName("EnableAutomaticQueueRegistration"));
		EnableAutomaticQueueRegistration->SetDataDisplayName(LOCTEXT("EnableAutomaticQueueRegistration", "Enable Automatic Queue Registration"));
		EnableAutomaticQueueRegistration->AddDynamicOption(TEXT("Enable"), LOCTEXT("Enable", "Enable"));
		EnableAutomaticQueueRegistration->AddDynamicOption(TEXT("Disable"), LOCTEXT("Disable", "Disable"));
		EnableAutomaticQueueRegistration->SetDefaultValueFromString(TEXT("Enable"));
		EnableAutomaticQueueRegistration->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentEnableAutomaticQueueRegistration));
		EnableAutomaticQueueRegistration->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentEnableAutomaticQueueRegistration));
		GameplayTabCollection->AddChildListData(EnableAutomaticQueueRegistration);
	}

	// Limit Client Transmission Rate
	{
		UListDataObjectString* LimitClientTransmissionRate = NewObject<UListDataObjectString>();
		LimitClientTransmissionRate->SetDataID(FName("LimitClientTransmissionRate"));
		LimitClientTransmissionRate->SetDataDisplayName(LOCTEXT("LimitClientTransmissionRate", "Limit Client Transmission Rate"));
		LimitClientTransmissionRate->AddDynamicOption(TEXT("Disable"), LOCTEXT("Enable", "Enable"));
		LimitClientTransmissionRate->AddDynamicOption(TEXT("Enable"), LOCTEXT("Enable", "Enable"));
		LimitClientTransmissionRate->SetDefaultValueFromString(TEXT("Disable"));
		LimitClientTransmissionRate->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentLimitClientTransmissionRate));
		LimitClientTransmissionRate->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentLimitClientTransmissionRate));
		LimitClientTransmissionRate->SetShouldApplySettingsImmediately(true);
		GameplayTabCollection->AddChildListData(LimitClientTransmissionRate);
	}

	RegisteredOptionTabCollections.Add(GameplayTabCollection);
}

void UOptionDataRegistry::InitControlCollectionTab()
{
	UListDataObjectCollection* ControlTabCollection = NewObject<UListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(LOCTEXT("Control", "Control"));

	RegisteredOptionTabCollections.Add(ControlTabCollection);
}
#undef LOCTEXT_NAMESPACE
