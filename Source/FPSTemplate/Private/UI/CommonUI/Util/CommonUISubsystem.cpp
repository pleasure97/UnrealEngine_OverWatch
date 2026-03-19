// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Util/CommonUISubsystem.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"
#include "Engine/AssetManager.h"
#include "UI/CommonUI/PrimaryLayoutWidget.h"
#include "UI/CommonUI/ActivatableBaseWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/CommonUI/ConfirmScreenWidget.h"
#include "OWGameplayTags.h"
#include "UI/CommonUI/Util/CommonUIFunctionLibrary.h"

UCommonUISubsystem* UCommonUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UCommonUISubsystem>(World->GetGameInstance()); 
	}

	return nullptr;
}

bool UCommonUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses; 
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UCommonUISubsystem::RegisterCreatedPrimaryLayoutWidget(UPrimaryLayoutWidget* InCreatedWidget)
{
	check(InCreatedWidget); 

	CreatedPrimaryLayoutWidget = InCreatedWidget; 

	CommonUIDebug::Print(TEXT("Primary Layout Widget Stored in Common UI Subsystem"));
}

void UCommonUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UActivatableBaseWidget> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UActivatableBaseWidget*)> AsyncPushStateCallback)
{
	check(!InSoftWidgetClass.IsNull()); 

	// Request Async Load 
	// Command to load widget assets from the hard disk into memory via "AssetManager"
	// Since this task runs on a separate thread or in the background, load on GameThread is reduced.
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		// After Asset Loading is Complete, Call Lambda Function 
		FStreamableDelegate::CreateLambda(
			[this, InSoftWidgetClass, InWidgetStackTag, AsyncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass && CreatedPrimaryLayoutWidget);
				// Found Widget Stack from Primary Layout Widget 
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayoutWidget->FindWidgetStackByTag(InWidgetStackTag);
				UActivatableBaseWidget* CreatedWidget = FoundWidgetStack->AddWidget<UActivatableBaseWidget>(
					LoadedWidgetClass,
					// State just before being fully 'pushed' onto the widget stack
					// At this point, the initial data required for the widget can be pre-set
					[AsyncPushStateCallback](UActivatableBaseWidget& CreatedWidgetInstance)
					{
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);
				// After the widget is successfully added to the stack
				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

void UCommonUISubsystem::PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMessage, const FText& Option1Text, const FText& Option2Text, const FText& Option3Text, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr; 

	switch (InScreenType)
	{
	case EConfirmScreenType::OK:
	{
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMessage, Option1Text);
		break;
	}
	case EConfirmScreenType::YesNo:
	{
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMessage, Option1Text, Option2Text);
		break;
	}
	case EConfirmScreenType::CancelYes:
	{
		CreatedInfoObject = UConfirmScreenInfoObject::CreateCancelYesScreen(InScreenTitle, InScreenMessage, Option1Text, Option2Text);
		break;
	}
	case EConfirmScreenType::OKCancel:
	{
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKCancelScreen(InScreenTitle, InScreenMessage, Option1Text, Option2Text);
		break;
	}
	case EConfirmScreenType::Unknown:
	{
		break;
	}
	default:
		break;
	}

	check(CreatedInfoObject);

	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

	PushSoftWidgetToStackAsync(
		GameplayTags.CommonUI_WidgetStack_Modal,
		UCommonUIFunctionLibrary::GetCommonUISoftWidgetClassByTag(GameplayTags.CommonUI_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushWidgetState, UActivatableBaseWidget* PushedWidget)
		{
			if (InPushWidgetState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UConfirmScreenWidget* CreatedConfirmScreen = CastChecked<UConfirmScreenWidget>(PushedWidget);
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
	);
}




