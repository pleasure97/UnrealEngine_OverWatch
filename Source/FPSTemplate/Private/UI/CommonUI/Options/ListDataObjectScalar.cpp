// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectScalar.h"
#include "UI/CommonUI/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObjectScalar::NoDecimal()
{
	FCommonNumberFormattingOptions CommonNumberFormattingOptions;
	CommonNumberFormattingOptions.MaximumFractionalDigits = 0;

	return CommonNumberFormattingOptions;
}

FCommonNumberFormattingOptions UListDataObjectScalar::WithDecimal(int32 NumFractionalDigits)
{
	FCommonNumberFormattingOptions CommonNumberFormattingOptions;
	CommonNumberFormattingOptions.MaximumFractionalDigits = NumFractionalDigits;

	return CommonNumberFormattingOptions;
}

float UListDataObjectScalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(OutputValueRange, DisplayValueRange, StringToFloat(DataDynamicGetter->GetValueAsString()));
	}

	return 0.f;
}

void UListDataObjectScalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(DisplayValueRange, OutputValueRange, InNewValue);
		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));

		NotifyListDataModified(this);
	}
}

bool UListDataObjectScalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}
	return false;
}

bool UListDataObjectScalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}
	return false;
}

void UListDataObjectScalar::OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);

	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}

float UListDataObjectScalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;
	LexFromString(OutConvertedValue, *InString);
	return OutConvertedValue;
}
