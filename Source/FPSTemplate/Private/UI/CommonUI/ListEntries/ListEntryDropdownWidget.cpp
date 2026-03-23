// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ListEntries/ListEntryDropdownWidget.h"
#include "Components/ComboBoxString.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"

void UListEntryDropdownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ComboBoxString_Options)
	{
		ComboBoxString_Options->OnSelectionChanged.AddDynamic(this, &UListEntryDropdownWidget::HandleSelectionChanged);
	}
}

void UListEntryDropdownWidget::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringEnumDataObject = CastChecked<UListDataObjectStringEnum>(InOwningListDataObject);

	ComboBoxString_Options->ClearOptions();

	for (const FText& Option : CachedOwningStringEnumDataObject->GetAvailableOptionsTextArray())
	{
		ComboBoxString_Options->AddOption(Option.ToString());
	}

	ComboBoxString_Options->SetSelectedOption(CachedOwningStringEnumDataObject->GetCurrentDisplayText().ToString());
}

void UListEntryDropdownWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringEnumDataObject)
	{
		ComboBoxString_Options->SetSelectedOption(CachedOwningStringEnumDataObject->GetCurrentDisplayText().ToString());
	}
}

void UListEntryDropdownWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType != ESelectInfo::Direct && CachedOwningStringEnumDataObject)
	{
		// TODO 

		SelectThisEntryWidget();
	}
}
