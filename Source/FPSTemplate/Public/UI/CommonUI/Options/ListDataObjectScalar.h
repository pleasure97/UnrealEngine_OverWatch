// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/Options/ListDataObjectValue.h"
#include "CommonNumericTextBlock.h"
#include "ListDataObjectScalar.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListDataObjectScalar : public UListDataObjectValue
{
	GENERATED_BODY()
	
public:
	LIST_DATA_ACCESORS(TRange<float>, DisplayValueRange)
	LIST_DATA_ACCESORS(TRange<float>, OutputValueRange)
	LIST_DATA_ACCESORS(float, SliderStepSize)
	LIST_DATA_ACCESORS(ECommonNumericType, DisplayNumericType)
	LIST_DATA_ACCESORS(FCommonNumberFormattingOptions, NumberFormattingOptions)

	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFractionalDigits);

	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(float InNewValue);

private:
	/* UListDataObjectBase */
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

	float StringToFloat(const FString& InString) const;

	TRange<float> DisplayValueRange = TRange<float>(0.f, 1.f); 
	TRange<float> OutputValueRange = TRange<float>(0.f, 1.f); 
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
};
