// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"

void UListEntryBaseWidget::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, IsListItemSelected());
}

void UListEntryBaseWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UListDataObjectBase>(ListItemObject));
}

void UListEntryBaseWidget::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &UListEntryBaseWidget::OnOwningListDataObjectModified);
	}
}

void UListEntryBaseWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{

}

void UListEntryBaseWidget::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
