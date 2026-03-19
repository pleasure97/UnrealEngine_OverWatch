// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Async/PushConfirmScreenAsyncAction.h"
#include "UI/CommonUI/Util/CommonUISubsystem.h"

UPushConfirmScreenAsyncAction* UPushConfirmScreenAsyncAction::PushConfirmScreen(const UObject* WorldContextObject, EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMessage, FText InOption1Text, FText InOption2Text, FText InOption3Text)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UPushConfirmScreenAsyncAction* Node = NewObject<UPushConfirmScreenAsyncAction>();

			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;
			Node->CachedOption1Text = InOption1Text;
			Node->CachedOption2Text = InOption2Text;
			Node->CachedOption3Text = InOption3Text;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}

void UPushConfirmScreenAsyncAction::Activate()
{
	UCommonUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(
		CachedScreenType,
		CachedScreenTitle,
		CachedScreenMessage,
		CachedOption1Text,
		CachedOption2Text,
		CachedOption3Text,
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			OnButtonClicked.Broadcast(ClickedButtonType);

			SetReadyToDestroy();
		}
	);
}
