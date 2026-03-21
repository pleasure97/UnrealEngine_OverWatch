// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/OptionDataRegistry.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"

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
	GraphicTabCollection->SetDataDisplayName(FText::FromString("Graphic"));

	RegisteredOptionTabCollections.Add(GraphicTabCollection);
}

void UOptionDataRegistry::InitAudioCollectionTab()
{
	UListDataObjectCollection* AudioTabCollection = NewObject<UListDataObjectCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

	// Volume Category
	{
		UListDataObjectCollection* VolumeCategoryCollection = NewObject<UListDataObjectCollection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Test Item for Category
		{
			UListDataObjectString* TestItem = NewObject<UListDataObjectString>();
			TestItem->SetDataID(FName("TestItem"));
			TestItem->SetDataDisplayName(FText::FromString(TEXT("Test Item")));

			VolumeCategoryCollection->AddChildListData(TestItem);
		}
	}

	RegisteredOptionTabCollections.Add(AudioTabCollection);
}

void UOptionDataRegistry::InitGameplayCollectionTab()
{
	UListDataObjectCollection* GameplayTabCollection = NewObject<UListDataObjectCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	// Enable Automatic Queue Registration
	{
		UListDataObjectString* EnableAutomaticQueueRegistration = NewObject<UListDataObjectString>();
		EnableAutomaticQueueRegistration->SetDataID(FName("EnableAutomaticQueueRegistration"));
		EnableAutomaticQueueRegistration->SetDataDisplayName(FText::FromString(TEXT("Enable Automatic Queue Registration")));
		EnableAutomaticQueueRegistration->AddDynamicOption(TEXT("Enable"), FText::FromString(TEXT("Enable")));
		EnableAutomaticQueueRegistration->AddDynamicOption(TEXT("Disable"), FText::FromString(TEXT("Disable")));
		EnableAutomaticQueueRegistration->SetDefaultValueFromString(TEXT("Enable"));
		EnableAutomaticQueueRegistration->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentEnableAutomaticQueueRegistration));
		EnableAutomaticQueueRegistration->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentEnableAutomaticQueueRegistration));
		GameplayTabCollection->AddChildListData(EnableAutomaticQueueRegistration);
	}

	// Limit Client Transmission Rate
	{
		UListDataObjectString* LimitClientTransmissionRate = NewObject<UListDataObjectString>();
		LimitClientTransmissionRate->SetDataID(FName("LimitClientTransmissionRate"));
		LimitClientTransmissionRate->SetDataDisplayName(FText::FromString(TEXT("Limit Client Transmission Rate")));
		LimitClientTransmissionRate->AddDynamicOption(TEXT("Disable"), FText::FromString(TEXT("Disable")));
		LimitClientTransmissionRate->AddDynamicOption(TEXT("Enable"), FText::FromString(TEXT("Enable")));
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
	ControlTabCollection->SetDataDisplayName(FText::FromString("Control"));

	RegisteredOptionTabCollections.Add(ControlTabCollection);
}
