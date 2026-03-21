// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Components/OWCommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "UI/CommonUI/Options/ListEntryMappingDataAsset.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "UI/CommonUI/Options/ListDataObjectCollection.h"

UUserWidget& UOWCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!IsDesignTime())
	{
		if (TSubclassOf<UListEntryBaseWidget> FoundWidgetClass =
			ListEntryMappingDataAsset->FindEntryWidgetClassByDataObject(CastChecked<UListDataObjectBase>(Item)))
		{
			return GenerateTypedEntry<UListEntryBaseWidget>(FoundWidgetClass, OwnerTable);
		}
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UOWCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return FirstSelectedItem->IsA<UListDataObjectCollection>();
}

#if WITH_EDITOR
void UOWCommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!ListEntryMappingDataAsset)
	{
		CompileLog.Error(FText::FromString(
			TEXT("List Entry Mapping Data Asset Has No Valid Data Asset Assigned") +
			GetClass()->GetName() +
			TEXT(" Needs Valid Data Asset to Function Properly")
		)); 
	}
}
#endif
