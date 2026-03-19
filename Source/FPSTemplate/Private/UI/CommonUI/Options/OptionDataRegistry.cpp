// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/OptionDataRegistry.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"

void UOptionDataRegistry::InitOptionDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGraphicCollectionTab();
	InitAudioCollectionTab();
	InitGameplayCollectionTab();
	InitControlCollectionTab();
}

void UOptionDataRegistry::InitGraphicCollectionTab()
{
	UListDataObjectCollection* GraphicTabCollection = NewObject<UListDataObjectCollection>(); 
	GraphicTabCollection->SetDataID(FName("GraphicTabCollection")); 
	GraphicTabCollection->SetDataDisplayName(FText::FromString(TEXT("Graphic")));

	RegisteredOptionTabCollections.Add(GraphicTabCollection);
}

void UOptionDataRegistry::InitAudioCollectionTab()
{
	UListDataObjectCollection* AudioTabCollection = NewObject<UListDataObjectCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	RegisteredOptionTabCollections.Add(AudioTabCollection);
}

void UOptionDataRegistry::InitGameplayCollectionTab()
{
	UListDataObjectCollection* GameplayTabCollection = NewObject<UListDataObjectCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	RegisteredOptionTabCollections.Add(GameplayTabCollection);
}

void UOptionDataRegistry::InitControlCollectionTab()
{
	UListDataObjectCollection* ControlTabCollection = NewObject<UListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionTabCollections.Add(ControlTabCollection);
}
