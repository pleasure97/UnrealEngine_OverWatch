// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "CommonInputSubsystem.h"

void UListEntryBaseWidget::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);

	if (bWasHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemExpanded() ? true : false);
	}
}

FReply UListEntryBaseWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UListEntryBaseWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UListDataObjectBase>(ListItemObject));
}

void UListEntryBaseWidget::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased(); 

	NativeOnListEntryWidgetHovered(false);
}

void UListEntryBaseWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	BP_OnToggleEntryWidgetHighlightState(bIsSelected);
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

	if (!InOwningListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnDependencyDataModified.AddUObject(this, &UListEntryBaseWidget::OnOwningDependencyDataObjectModified);
	}

	OnToggleEditableState(InOwningListDataObject->IsDataCurrentlyEditable());

	CachedOwningListDataObject = InOwningListDataObject;
}

void UListEntryBaseWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{

}

void UListEntryBaseWidget::OnOwningDependencyDataObjectModified(UListDataObjectBase* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningListDataObject)
	{
		OnToggleEditableState(CachedOwningListDataObject->IsDataCurrentlyEditable());
	}
}

void UListEntryBaseWidget::OnToggleEditableState(bool bIsEditable)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
	}
}

void UListEntryBaseWidget::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
