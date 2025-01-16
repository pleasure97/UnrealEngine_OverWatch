// Fill out your copyright notice in the Description page of Project Settings.


#include "OWAssetManager.h"
#include "OWGameplayTags.h"
#include "AbilitySystemGlobals.h"

UOWAssetManager& UOWAssetManager::Get()
{
	check(GEngine); 
	UOWAssetManager* OWAssetManager = Cast<UOWAssetManager>(GEngine->AssetManager);
	return *OWAssetManager;
}

void UOWAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading(); 

	FOWGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data. 
	UAbilitySystemGlobals::Get().InitGlobalData(); 
}
