// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/ListEntries/ListEntrySelectorWidget.h"
#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Components/OWCommonRotator.h"
#include "UI/CommonUI/Components/OWCommonButtonBase.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"

void UListEntrySelectorWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &UListEntrySelectorWidget::OnPreviousOptionClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &UListEntrySelectorWidget::OnNextOptionClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this](){ SelectThisEntryWidget(); });
}

void UListEntrySelectorWidget::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObjectString>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText()); 
}

void UListEntrySelectorWidget::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UListEntrySelectorWidget::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	CommonButton_PreviousOption->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
	CommonButton_NextOption->SetIsEnabled(bIsEditable);
}

void UListEntrySelectorWidget::OnPreviousOptionClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UListEntrySelectorWidget::OnNextOptionClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisEntryWidget();
}
