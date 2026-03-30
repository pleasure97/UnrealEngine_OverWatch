// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectString.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"

#define LOCTEXT_NAMESPACE "OptionsUI"
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

void UListDataObjectString::OnRotatorInitialValueChanged(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	if (FoundIndex != INDEX_NONE && AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		CurrentDisplayText = InNewSelectedText;
		CurrentStringValue = AvailableOptionsStringArray[FoundIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue); 

			NotifyListDataModified(this);
		}
	}
}

void UListDataObjectString::SetSelectedOption(const FString& InSelectedOption)
{
	if(AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	TrySetStringValueFromDisplayString(InSelectedOption);

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

bool UListDataObjectString::CanSetToForcedStringValue(const FString& InForcedStringValue) const
{
	return CurrentStringValue != InForcedStringValue;
}

void UListDataObjectString::OnSetToForcedStringValue(const FString& InForcedStringValue)
{
	CurrentStringValue = InForcedStringValue;
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
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
		CurrentDisplayText = LOCTEXT("Invalid Option", "Invalid Option");
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

bool UListDataObjectString::TrySetStringValueFromDisplayString(const FString& InStringValue)
{
	CurrentDisplayText = FText::FromString(InStringValue);
	int32 FoundIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[this](const FText& Item) 
		{
			return Item.EqualTo(CurrentDisplayText);
		});
	if (AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		CurrentStringValue = AvailableOptionsStringArray[FoundIndex];
		return true;
	}
	return false;
}

/*
 *  ListDataObject - String Bool
 */

void UListDataObjectStringBool::OverrideTrueDisplayText(const FText& InNewTrueDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, InNewTrueDisplayText);
	}
}

void UListDataObjectStringBool::OverrideFalseDisplayText(const FText& InNewFalseDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, InNewFalseDisplayText);
	}
}

void UListDataObjectStringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UListDataObjectStringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UListDataObjectStringBool::OnDataObjectInitialized()
{
	TryInitBoolValues();

	Super::OnDataObjectInitialized();
}

void UListDataObjectStringBool::TryInitBoolValues()
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, LOCTEXT("ON", "ON"));
	}

	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, LOCTEXT("OFF", "OFF"));
	}
}

/*
 *  ListDataObject - String Integer
 */
void UListDataObjectStringInteger::AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText)
{
	AddDynamicOption(LexToString(InIntegerValue), InDisplayText);
}

void UListDataObjectStringInteger::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = LOCTEXT("Custom", "Custom");
	}
}

void UListDataObjectStringInteger::OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		if (CurrentStringValue == DataDynamicGetter->GetValueAsString())
		{
			return;
		}

		CurrentStringValue = DataDynamicGetter->GetValueAsString();

		if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
		{
			CurrentDisplayText = LOCTEXT("Custom", "Custom");
		}

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}

	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}

#undef LOCTEXT_NAMESPACE

