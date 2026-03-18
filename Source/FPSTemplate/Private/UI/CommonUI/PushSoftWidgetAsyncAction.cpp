// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/PushSoftWidgetAsyncAction.h"
#include "UI/CommonUI/CommonUISubsystem.h"
#include "UI/CommonUI/ActivatableBaseWidget.h"

UPushSoftWidgetAsyncAction* UPushSoftWidgetAsyncAction::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass, UPARAM(meta = (Categories = "CommonUI.WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
    checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class")); 

    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            UPushSoftWidgetAsyncAction* Node = NewObject<UPushSoftWidgetAsyncAction>();

            // Cache Owning World, Player Controller, Soft Widget Class, Widget Stack Tag, ...
            Node->CachedOwningWorld = World;
            Node->CachedOwningPlayerController = OwningPlayerController;
            Node->CachedSoftWidgetClass = InSoftWidgetClass;
            Node->CachedWidgetStackTag = InWidgetStackTag;
            Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

            Node->RegisterWithGameInstance(World); 

            return Node;
        }
    }
    return nullptr;
}

void UPushSoftWidgetAsyncAction::Activate()
{
    UCommonUISubsystem* CommonUISubsystem = UCommonUISubsystem::Get(CachedOwningWorld.Get()); 

    if (IsValid(CommonUISubsystem))
    {
        CommonUISubsystem->PushSofWidgetToStackAsync(
            CachedWidgetStackTag,
            CachedSoftWidgetClass,
            [this](EAsyncPushWidgetState InPushState, UActivatableBaseWidget* PushedWidget)
            {
                switch (InPushState)
                {
                case EAsyncPushWidgetState::OnCreatedBeforePush:
                {
                    PushedWidget->SetOwningPlayer(CachedOwningPlayerController.Get()); 
                    OnWidgetCreatedBeforePush.Broadcast(PushedWidget); 
                    break;
                }
                case EAsyncPushWidgetState::AfterPush:
                {
                    AfterPush.Broadcast(PushedWidget);
                    if (bCachedFocusOnNewlyPushedWidget)
                    {
                        if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
                        {
                            WidgetToFocus->SetFocus(); 
                        }
                    }

                    SetReadyToDestroy(); 

                    break;
                }
                default:
                    break;
                }
            });
    }
}
