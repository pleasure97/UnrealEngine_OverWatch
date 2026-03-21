// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"

void UListDataObjectString::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
}

void UListDataObjectString::AdvanceToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextDisplayIndex = CurrentDisplayIndex + 1;

	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(NextDisplayIndex);

	if (bIsNextIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[NextDisplayIndex];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		CommonUIDebug::Print(TEXT("DataDynamicSetter is used. The latest value from Getter : ") + DataDynamicGetter->GetValueAsString());

		NotifyListDataModified(this);
	}
}

void UListDataObjectString::BackToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousDisplayIndex = CurrentDisplayIndex - 1;

	const bool bIsPreviousIndexValid = AvailableOptionsStringArray.IsValidIndex(PreviousDisplayIndex);

	if (bIsPreviousIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[PreviousDisplayIndex];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		CommonUIDebug::Print(TEXT("DataDynamicSetter is used. The latest value from Getter : ") + DataDynamicGetter->GetValueAsString());

		NotifyListDataModified(this);
	}
}

bool UListDataObjectString::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && (CurrentStringValue != GetDefaultValueAsString());
}

bool UListDataObjectString::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();

		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}

	return false;
}

void UListDataObjectString::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	// Read from Saved String Value and Use it to Set Current String Value
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
}

bool UListDataObjectString::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);

	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];

		return true;
	}
	return false;
}
