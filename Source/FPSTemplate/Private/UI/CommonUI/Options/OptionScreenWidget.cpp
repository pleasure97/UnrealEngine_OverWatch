// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/OptionScreenWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"
#include "UI/CommonUI/Options/OptionDataRegistry.h"
#include "UI/CommonUI/Components/OWCommonTabListWidgetBase.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"

void UOptionScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized(); 

	if (!ResetAction.IsNull())
	{
		// Register "Reset Action" UI Binding
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true, /*bShouldDisplayInActionBar*/
				FSimpleDelegate::CreateUObject(this, &UOptionScreenWidget::OnResetBoundActionTriggered)
			)
		);
	}

	// Register "Back Action" UI Binding
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true, /*bShouldDisplayInActionBar*/
			FSimpleDelegate::CreateUObject(this, &UOptionScreenWidget::OnBackBoundActionTriggered)
		)
	);
}

void UOptionScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (!IsValid(TabListWidget_OptionsTab))
	{
		return;
	}

	for (UListDataObjectCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}

		const FName TabID = TabCollection->GetDataID();
		
		if (TabListWidget_OptionsTab->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		TabListWidget_OptionsTab->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

UOptionDataRegistry* UOptionScreenWidget::GetOrCreateDataRegistry()
{
	if (!OwningDataRegistry)
	{
		OwningDataRegistry = NewObject<UOptionDataRegistry>(); 
		OwningDataRegistry->InitOptionDataRegistry(GetOwningLocalPlayer()); 
	}

	checkf(OwningDataRegistry, TEXT("Data Registry for Option Screen is Not Valid"));

	return OwningDataRegistry;
}

void UOptionScreenWidget::OnResetBoundActionTriggered()
{
	CommonUIDebug::Print(TEXT("Reset Bound Action Triggered"));
}

void UOptionScreenWidget::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
