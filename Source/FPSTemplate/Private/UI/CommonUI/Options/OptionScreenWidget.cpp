// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/OptionScreenWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"
#include "UI/CommonUI/Options/OptionDataRegistry.h"
#include "UI/CommonUI/Components/OWCommonTabListWidgetBase.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"
#include "UI/CommonUI/Components/OWCommonListView.h"
#include "UI/CommonUI/Util/OWGameUserSettings.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "UI/CommonUI/Util/CommonUISubsystem.h"
#include "UI/CommonUI/Components/OWCommonButtonBase.h"

#define LOCTEXT_NAMESPACE "OptionsUI"

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

			TabListWidget_OptionsTab->OnTabSelected.AddUniqueDynamic(this, &UOptionScreenWidget::OnOptionTabSelected);

			CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &UOptionScreenWidget::OnListViewItemHovered);
			CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &UOptionScreenWidget::OnListViewItemSelected);
}

void UOptionScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

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

void UOptionScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UOWGameUserSettings::Get()->ApplySettings(true);
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
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTab->GetTabButtonBaseByID(TabListWidget_OptionsTab->GetActiveTab());

	const FString SelectedTabButtonName = CastChecked<UOWCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UCommonUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::NoYes,
		FText::Format(LOCTEXT("SettingsResetWarning", "Are you sure you want to reset all the settings under the {0}?"), FText::FromString(SelectedTabButtonName)),
		FText::GetEmpty(),
		LOCTEXT("No", "No"),
		LOCTEXT("Yes", "Yes"),
		FText::GetEmpty(),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType == EConfirmScreenButtonType::Confirmed)
			{
				bIsResettingData = true;
				bool bHasDataFailedToReset = false;

				for (UListDataObjectBase* DataToReset : ResettableDataArray)
				{
					if (!DataToReset)
					{
						continue;
					}

					if (DataToReset->TryResetBackToDefaultValue())
					{
						CommonUIDebug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" was reset"));
					}
					else
					{
						bHasDataFailedToReset = true;
						CommonUIDebug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
					}
				}

				if (!bHasDataFailedToReset)
				{
					ResettableDataArray.Empty();
					RemoveActionBinding(ResetActionHandle);
				}
				
				bIsResettingData = false;
			}
		}
	);
}

void UOptionScreenWidget::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UOptionScreenWidget::OnOptionTabSelected(FName TabId)
{
	TArray<UListDataObjectBase*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UListDataObjectBase* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &UOptionScreenWidget::OnListViewListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UOptionScreenWidget::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}
	
	UListEntryBaseWidget* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UListEntryBaseWidget>(InHoveredItem);

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
}

void UOptionScreenWidget::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

}

void UOptionScreenWidget::OnListViewListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
#undef LOCTEXT_NAMESPACE
