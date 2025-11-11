// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DSSystemStatics.h"
#include "Game/DSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Data/API/InteractiveAPIData.h"

void UDSSystemStatics::PlayNextGame(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull); 
	if (World == nullptr)
	{
		return; 
	}

	const FWorldContext& WorldContext = GEngine->GetWorldContextFromWorldChecked(World); 
	FURL LastURL = WorldContext.LastURL; 

#if WITH_EDITOR
	// To Transition during Play-in-Editor, we need to Strip the PIE Prefix from Maps. 
	LastURL.Map = UWorld::StripPIEPrefixFromPackageName(LastURL.Map, WorldContext.World()->StreamingLevelsPrefix); 
#endif

	// Add Seamless Travel Option as we want to keep clients connected. 
	// If Seamless Travel is Disabled, this will fall back to Hard Travel. 
	LastURL.AddOption(TEXT("SeamlessTravel")); 

	FString URL = LastURL.ToString(); 
	// Remove the Host / Port Info 
	URL.RemoveFromStart(LastURL.GetHostPortString()); 

	World->ServerTravel(URL, /* bAbsolute */ false, /* bShouldSkipGameNotify */ false); 
}

const FString UDSSystemStatics::GetAPIUrlFromDataAsset(const UObject* WorldContextObject)
{
	// Get Game State and Cast Game State to DS Game State
	if (ADSGameState* DSGameState = Cast<ADSGameState>(UGameplayStatics::GetGameState(WorldContextObject)))
	{
		// Get API URL of Interactive API Data Asset from DS Game State
		if (IsValid(DSGameState->InteractiveAPIDataAsset))
		{
			return DSGameState->InteractiveAPIDataAsset->GetAPIUrl();
		}
	}
	return FString();
}

const FString UDSSystemStatics::GetAPIKeyFromDataAsset(const UObject* WorldContextObject)
{
	// Get Game State and Cast Game State to DS Game State
	if (ADSGameState* DSGameState = Cast<ADSGameState>(UGameplayStatics::GetGameState(WorldContextObject)))
	{
		// Get API Key of Interactive API Data Asset from DS Game State
		if (IsValid(DSGameState->InteractiveAPIDataAsset))
		{
			return DSGameState->InteractiveAPIDataAsset->GetAPIKey();
		}
	}
	return FString();
}

const FString UDSSystemStatics::GetAPIModelFromDataAsset(const UObject* WorldContextObject)
{
	// Get Game State and Cast Game State to DS Game State
	if (ADSGameState* DSGameState = Cast<ADSGameState>(UGameplayStatics::GetGameState(WorldContextObject)))
	{
		// Get API Model of Interactive API Data Asset from DS Game State
		if (IsValid(DSGameState->InteractiveAPIDataAsset))
		{
			return DSGameState->InteractiveAPIDataAsset->GetAPIModel();
		}
	}
	return FString();
}



