// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/OptionDataRegistry.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Options/ListDataObjectScalar.h"
#include "UI/CommonUI/Options/ListDataObjectStringResolution.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"
#include "Internationalization/StringTableRegistry.h"

#define LOCTEXT_NAMESPACE "OptionsUI"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UOWGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Content/Blueprints/UI/OW/CommonUI/StringTables/ST_OptionsScreenDescription", InKey)

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

	UListDataObjectStringEnum* CreatedDisplayMode = nullptr;
	UListDataObjectStringInteger* CreatedGraphicQuality = nullptr;

	// Display Category
	{
		UListDataObjectCollection* DisplayCategoryCollection = NewObject<UListDataObjectCollection>(); 
		DisplayCategoryCollection->SetDataID(FName("DisplayTabCollection")); 
		DisplayCategoryCollection->SetDataDisplayName(LOCTEXT("Display", "Display"));

		GraphicTabCollection->AddChildListData(DisplayCategoryCollection); 

		// Edit Condition - Packaged Build
		FOptionDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]() -> bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);

		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in packaged build.</>"));

		// Display Mode
		{
			UListDataObjectStringEnum* DisplayMode = NewObject<UListDataObjectStringEnum>(); 
			DisplayMode->SetDataID(FName("DisplayMode")); 
			DisplayMode->SetDataDisplayName(LOCTEXT("DisplayMode", "Display Mode")); 
			// TODO - DisplayMode->SetDescriptionRichText(GET_DESCRIPTION("DisplayModeKey"));
			DisplayMode->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("FullScreen", "Full Screen"));
			DisplayMode->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("BorderlessWindow", "Borderless Window"));
			DisplayMode->AddEnumOption(EWindowMode::Windowed, LOCTEXT("Windowed", "Windowed"));
			
			DisplayMode->SetDefaultValueFromEnumOption(EWindowMode::Fullscreen);

			DisplayMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			DisplayMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));

			DisplayMode->SetShouldApplySettingsImmediately(true);

			DisplayMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedDisplayMode = DisplayMode;

			DisplayCategoryCollection->AddChildListData(DisplayMode);
		}

		// Resolution
		{
			UListDataObjectStringResolution* ScreenResolution = NewObject<UListDataObjectStringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(LOCTEXT("ScreenResolution", "Screen Resolution"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionDataEditConditionDescriptor DisplayModeEditCondition;
			DisplayModeEditCondition.SetEditConditionFunc(
				[CreatedDisplayMode]() -> bool
				{
					const bool bIsBorderlessWindow = 
						CreatedDisplayMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

					return !bIsBorderlessWindow;
				}
			);

			DisplayModeEditCondition.SetDisabledRichReason(
				TEXT("\n\n<Disabled>Screen resolution is not adjustable when the 'Window Mode' is set borderless window. The value must match with maximum allowed resolution.</>"));
			DisplayModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(DisplayModeEditCondition);
			ScreenResolution->AddEditDependencyData(CreatedDisplayMode);
			
			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	// Graphic Category
	{
		UListDataObjectCollection* GraphicCategoryCollection = NewObject<UListDataObjectCollection>();
		GraphicCategoryCollection->SetDataID(FName("GraphicCategoryCollection"));
		GraphicCategoryCollection->SetDataDisplayName(LOCTEXT("GraphicCategoryCollection", "Graphic"));

		GraphicTabCollection->AddChildListData(GraphicCategoryCollection);

		// View Distance
		{
			UListDataObjectStringInteger* ViewDistance = NewObject<UListDataObjectStringInteger>();

			ViewDistance->SetDataID(FName("AntiAliasing"));
			ViewDistance->SetDataDisplayName(LOCTEXT("ViewDistance", "View Distance"));
			ViewDistance->AddIntegerOption(0, LOCTEXT("View Distance Low", "Low"));
			ViewDistance->AddIntegerOption(1, LOCTEXT("View Distance Medium", "Medium"));
			ViewDistance->AddIntegerOption(2, LOCTEXT("View Distance High", "High"));
			ViewDistance->AddIntegerOption(3, LOCTEXT("View Distance Very High", "Very High"));
			ViewDistance->AddIntegerOption(4, LOCTEXT("View Distance Epic", "Epic"));
			ViewDistance->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistance->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistance->SetShouldApplySettingsImmediately(true);

			GraphicCategoryCollection->AddChildListData(ViewDistance);
		}

		// Frame Rate Limit
		{
			//UListDataObjectScalar* FrameRateLimit = NewObject<UListDataObjectScalar>();
			//FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			//FrameRateLimit->SetDataDisplayName(LOCTEXT("FrameRateLimit", "Frame Rate Limit"));
			//FrameRateLimit->SetDisplayValueRange(TRange<float>(60.f, 540.f));
			//FrameRateLimit->SetOutputValueRange(TRange<float>(60.f, 540.f)); // Default Value for Unreal Has is 2.2f
			//FrameRateLimit->SetSliderStepSize(1.f);
			//FrameRateLimit->SetDisplayNumericType(ECommonNumericType::Number);
			//FrameRateLimit->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			//FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			//FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));

			//FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			//FrameRateLimit->SetShouldApplySettingsImmediately(true);

			//GraphicCategoryCollection->AddChildListData(FrameRateLimit);
		}

		// Vertical Sync
		{
			UListDataObjectStringBool* VerticalSync = NewObject<UListDataObjectStringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(LOCTEXT("VerticalSync", "Vertical Sync"));
			VerticalSync->OverrideTrueDisplayText(LOCTEXT("Enabled", "Enabled"));
			VerticalSync->OverrideFalseDisplayText(LOCTEXT("Disabled", "Disabled"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplySettingsImmediately(true);

			// Full Screen Only Condition 
			FOptionDataEditConditionDescriptor FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc(
				[CreatedDisplayMode]() -> bool
				{
					return CreatedDisplayMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature only works if the 'Window Mode' is set to 'Fullscreen'.</>"));
			FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullScreenOnlyCondition);
			GraphicCategoryCollection->AddChildListData(VerticalSync);
		}

		// Gamma Correction
		{
			UListDataObjectScalar* GammaCorrection = NewObject<UListDataObjectScalar>();
			GammaCorrection->SetDataID(FName("GammaCorrection"));
			GammaCorrection->SetDataDisplayName(LOCTEXT("GammaCorrection", "Gamma Correction")); 
			GammaCorrection->SetDisplayValueRange(TRange<float>(1.7f, 2.7f)); 
			GammaCorrection->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); // Default Value for Unreal Has is 2.2f
			GammaCorrection->SetSliderStepSize(0.01f);
			GammaCorrection->SetDisplayNumericType(ECommonNumericType::Number);
			GammaCorrection->SetNumberFormattingOptions(UListDataObjectScalar::WithDecimal(2));
			GammaCorrection->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGammaCorrection));
			GammaCorrection->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGammaCorrection));

			GammaCorrection->SetDefaultValueFromString(LexToString(2.2f));
			GammaCorrection->SetShouldApplySettingsImmediately(true);

			GraphicCategoryCollection->AddChildListData(GammaCorrection);
		}
	}

	// Graphic Quality Category 
	{
		UListDataObjectCollection* GraphicQualityCategoryCollection = NewObject<UListDataObjectCollection>();
		GraphicQualityCategoryCollection->SetDataID(FName("GraphicQualityCategoryCollection"));
		GraphicQualityCategoryCollection->SetDataDisplayName(LOCTEXT("GraphicQualityCategory", "Graphic Quality"));

		GraphicTabCollection->AddChildListData(GraphicQualityCategoryCollection);


		// Graphic Quality 
		{
			UListDataObjectStringInteger* GraphicQuality = NewObject<UListDataObjectStringInteger>();

			GraphicQuality->SetDataID(FName("GraphicQuality"));
			GraphicQuality->SetDataDisplayName(LOCTEXT("GraphicQuality", "Graphic Quality"));
			GraphicQuality->AddIntegerOption(0, LOCTEXT("Graphic Quality Low", "Low"));
			GraphicQuality->AddIntegerOption(1, LOCTEXT("Graphic Quality Medium", "Medium"));
			GraphicQuality->AddIntegerOption(2, LOCTEXT("Graphic Quality High", "High"));
			GraphicQuality->AddIntegerOption(3, LOCTEXT("Graphic Quality Very High", "Very High"));
			GraphicQuality->AddIntegerOption(4, LOCTEXT("Graphic Quality Epic", "Epic"));
			GraphicQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			GraphicQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			GraphicQuality->SetShouldApplySettingsImmediately(true);

			GraphicQualityCategoryCollection->AddChildListData(GraphicQuality);

			CreatedGraphicQuality = GraphicQuality;
		}

		// Anti Aliasing
		{
			UListDataObjectStringInteger* AntiAliasing = NewObject<UListDataObjectStringInteger>();

			AntiAliasing->SetDataID(FName("AntiAliasing"));
			AntiAliasing->SetDataDisplayName(LOCTEXT("AntiAliasing", "Anti Aliasing"));
			AntiAliasing->AddIntegerOption(0, LOCTEXT("Anti Aliasing Low", "Low"));
			AntiAliasing->AddIntegerOption(1, LOCTEXT("Anti Aliasing Medium", "Medium"));
			AntiAliasing->AddIntegerOption(2, LOCTEXT("Anti Aliasing High", "High"));
			AntiAliasing->AddIntegerOption(3, LOCTEXT("Anti Aliasing Very High", "Very High"));
			AntiAliasing->AddIntegerOption(4, LOCTEXT("Anti Aliasing Epic", "Epic"));
			AntiAliasing->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasing->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasing->SetShouldApplySettingsImmediately(true);

			AntiAliasing->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(AntiAliasing);

			GraphicQualityCategoryCollection->AddChildListData(AntiAliasing);
		}

		// Texture Quality
		{
			UListDataObjectStringInteger* TextureQuality = NewObject<UListDataObjectStringInteger>();

			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(LOCTEXT("TextureQuality", "Texture Quality"));
			TextureQuality->AddIntegerOption(0, LOCTEXT("Texture Quality Low", "Low"));
			TextureQuality->AddIntegerOption(1, LOCTEXT("Texture Quality Medium", "Medium"));
			TextureQuality->AddIntegerOption(2, LOCTEXT("Texture Quality High", "High"));
			TextureQuality->AddIntegerOption(3, LOCTEXT("Texture Quality Very High", "Very High"));
			TextureQuality->AddIntegerOption(4, LOCTEXT("Texture Quality Epic", "Epic"));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(TextureQuality);

			GraphicQualityCategoryCollection->AddChildListData(TextureQuality);
		}

		// Shadow Quality
		{
			UListDataObjectStringInteger* ShadowQuality = NewObject<UListDataObjectStringInteger>();

			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(LOCTEXT("ShadowQuality", "Shadow Quality"));
			ShadowQuality->AddIntegerOption(0, LOCTEXT("Shadow Quality Low", "Low"));
			ShadowQuality->AddIntegerOption(1, LOCTEXT("Shadow Quality Medium", "Medium"));
			ShadowQuality->AddIntegerOption(2, LOCTEXT("Shadow Quality High", "High"));
			ShadowQuality->AddIntegerOption(3, LOCTEXT("Shadow Quality Very High", "Very High"));
			ShadowQuality->AddIntegerOption(4, LOCTEXT("Shadow Quality Epic", "Epic"));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(ShadowQuality);

			GraphicQualityCategoryCollection->AddChildListData(ShadowQuality);
		}

		// Visual Effect Quality
		{
			UListDataObjectStringInteger* VisualEffectQuality = NewObject<UListDataObjectStringInteger>();

			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(LOCTEXT("VisualEffectQuality", "Visual Effect Quality"));
			VisualEffectQuality->AddIntegerOption(0, LOCTEXT("Visual Effect Quality Low", "Low"));
			VisualEffectQuality->AddIntegerOption(1, LOCTEXT("Visual Effect Quality Medium", "Medium"));
			VisualEffectQuality->AddIntegerOption(2, LOCTEXT("Visual Effect Quality High", "High"));
			VisualEffectQuality->AddIntegerOption(3, LOCTEXT("Visual Effect Quality Very High", "Very High"));
			VisualEffectQuality->AddIntegerOption(4, LOCTEXT("Visual Effect Quality Epic", "Epic"));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectQuality->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(VisualEffectQuality);

			GraphicQualityCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		// Global Illumination Quality
		{
			UListDataObjectStringInteger* GlobalIlluminationQuality = NewObject<UListDataObjectStringInteger>();

			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(LOCTEXT("GlobalIlluminationQuality", "Global Illumination Quality"));
			GlobalIlluminationQuality->AddIntegerOption(0, LOCTEXT("Global Illumination Quality Low", "Low"));
			GlobalIlluminationQuality->AddIntegerOption(1, LOCTEXT("Global Illumination Quality Medium", "Medium"));
			GlobalIlluminationQuality->AddIntegerOption(2, LOCTEXT("Global Illumination Quality High", "High"));
			GlobalIlluminationQuality->AddIntegerOption(3, LOCTEXT("Global Illumination Quality Very High", "Very High"));
			GlobalIlluminationQuality->AddIntegerOption(4, LOCTEXT("Global Illumination Quality Epic", "Epic"));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicQualityCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		// Reflection Quality
		{
			UListDataObjectStringInteger* ReflectionQuality = NewObject<UListDataObjectStringInteger>();

			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(LOCTEXT("ReflectionQuality", "Reflection Quality"));
			ReflectionQuality->AddIntegerOption(0, LOCTEXT("Reflection Quality Low", "Low"));
			ReflectionQuality->AddIntegerOption(1, LOCTEXT("Reflection Quality Medium", "Medium"));
			ReflectionQuality->AddIntegerOption(2, LOCTEXT("Reflection Quality High", "High"));
			ReflectionQuality->AddIntegerOption(3, LOCTEXT("Reflection Quality Very High", "Very High"));
			ReflectionQuality->AddIntegerOption(4, LOCTEXT("Reflection Quality Epic", "Epic"));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(ReflectionQuality);

			GraphicQualityCategoryCollection->AddChildListData(ReflectionQuality);
		}

		// Post Processing Quality
		{
			UListDataObjectStringInteger* PostProcessingQuality = NewObject<UListDataObjectStringInteger>();

			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(LOCTEXT("PostProcessingQuality", "PostProcessing Quality"));
			PostProcessingQuality->AddIntegerOption(0, LOCTEXT("PostProcessing Quality Low", "Low"));
			PostProcessingQuality->AddIntegerOption(1, LOCTEXT("PostProcessing Quality Medium", "Medium"));
			PostProcessingQuality->AddIntegerOption(2, LOCTEXT("PostProcessing Quality High", "High"));
			PostProcessingQuality->AddIntegerOption(3, LOCTEXT("PostProcessing Quality Very High", "Very High"));
			PostProcessingQuality->AddIntegerOption(4, LOCTEXT("PostProcessing Quality Epic", "Epic"));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedGraphicQuality);
			CreatedGraphicQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicQualityCategoryCollection->AddChildListData(PostProcessingQuality);
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

	// Find Match Category
	{
		UListDataObjectCollection* FindMatchCategoryCollection = NewObject<UListDataObjectCollection>();
		FindMatchCategoryCollection->SetDataID(FName("FindMatchCategoryCollection"));
		FindMatchCategoryCollection->SetDataDisplayName(LOCTEXT("Find Match Category", "Find Match"));

		GameplayTabCollection->AddChildListData(FindMatchCategoryCollection);
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
			FindMatchCategoryCollection->AddChildListData(EnableAutomaticQueueRegistration);
		}
	}

	// Network Category
	{
		UListDataObjectCollection* NetworkCategoryCollection = NewObject<UListDataObjectCollection>();
		NetworkCategoryCollection->SetDataID(FName("NetworkCategoryCollection"));
		NetworkCategoryCollection->SetDataDisplayName(LOCTEXT("Network Category", "Network"));

		GameplayTabCollection->AddChildListData(NetworkCategoryCollection);

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
			NetworkCategoryCollection->AddChildListData(LimitClientTransmissionRate);
		}
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
