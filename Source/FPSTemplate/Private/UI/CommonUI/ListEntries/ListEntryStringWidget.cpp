// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ListEntries/ListEntryStringWidget.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Components/OWCommonRotator.h"
#include "UI/CommonUI/Components/OWCommonButtonBase.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"

void UListEntryStringWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &UListEntryStringWidget::OnPreviousOptionClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &UListEntryStringWidget::OnNextOptionClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this](){ SelectThisEntryWidget(); });
}

void UListEntryStringWidget::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObjectString>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText()); 
}

void UListEntryStringWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UListEntryStringWidget::OnPreviousOptionClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UListEntryStringWidget::OnNextOptionClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisEntryWidget();
}
