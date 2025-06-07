// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingTask.h"
#include "LoadingScreenManager.h"

ULoadingTask* ULoadingTask::CreateLoadingScreenTask(UObject* WorldContextObject, const FString& ShowLoadingScreenReason)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull); 
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr; 
	ULoadingScreenManager* LoadingScreenManager = GameInstance ? GameInstance->GetSubsystem<ULoadingScreenManager>() : nullptr; 

	if (LoadingScreenManager)
	{
		ULoadingTask* NewLoadingTask = NewObject<ULoadingTask>(LoadingScreenManager); 
		NewLoadingTask->SetShowLoadingScreenReason(ShowLoadingScreenReason); 
		LoadingScreenManager->RegisterLoadingProcessor(NewLoadingTask); 

		return NewLoadingTask; 
	}

	return nullptr; 
}

void ULoadingTask::Unregister()
{
	ULoadingScreenManager* LoadingScreenManager = Cast<ULoadingScreenManager>(GetOuter()); 
	LoadingScreenManager->UnregisterLoadingProcessor(this); 
}

void ULoadingTask::SetShowLoadingScreenReason(const FString& InReason)
{
	Reason = InReason; 
}

bool ULoadingTask::ShouldShowLoadingScreen(FString& OutReason) const
{
	OutReason = Reason; 
	return true; 
}


