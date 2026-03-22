// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/ListEntries/ListEntryBaseWidget.h"
#include "ListEntryScalarWidget.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;
class UListDataObjectScalar;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNaiveTick))
class FPSTEMPLATE_API UListEntryScalarWidget : public UListEntryBaseWidget
{
	GENERATED_BODY()
	
protected:
	/* UWidget */
	virtual void NativeOnInitialized() override;

	/* UListEntryBaseWidget */
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject) override;

	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;

private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	/* Bound Widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_SettingSlider;
	/* Bound Widgets End */

	UPROPERTY(Transient)
	UListDataObjectScalar* CachedOwningScalarDataObject;
};
