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

	CachedOwningStringDataObject = CastChecked<UListDataObjectString>(InOwningListDataObject);
	
	ComboBoxString_Options->ClearOptions();

	for (const FText& Option : CachedOwningStringDataObject->GetAvailableOptionsTextArray())
	{
		ComboBoxString_Options->AddOption(Option.ToString());
	}

	ComboBoxString_Options->SetSelectedOption(CachedOwningStringDataObject->GetCurrentDisplayText().ToString());
}

void UListEntryDropdownWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		ComboBoxString_Options->SetSelectedOption(CachedOwningStringDataObject->GetCurrentDisplayText().ToString());
	}
}

void UListEntryDropdownWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType != ESelectInfo::Direct && CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->SetSelectedOption(SelectedItem); 

		SelectThisEntryWidget();
	}
}
