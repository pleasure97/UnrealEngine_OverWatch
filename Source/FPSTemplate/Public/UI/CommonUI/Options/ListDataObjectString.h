// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/Options/ListDataObjectValue.h"
#include "ListDataObjectString.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UListDataObjectString : public UListDataObjectValue
{
	GENERATED_BODY()
	
public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText); 
	
	void AdvanceToNextOption();
	void BackToPreviousOption();
	void OnRotatorInitialValueChanged(const FText& InNewSelectedText);
	void SetSelectedOption(const FString& InSelectedOption);

	const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FText GetCurrentDisplayText() const { return CurrentDisplayText; }

	/* UListDataObjectBase */
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual bool CanSetToForcedStringValue(const FString& InForcedStringValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedStringValue) override;

protected:
	/* UListDataObjectBase Begins */
	virtual void OnDataObjectInitialized() override;
	/* UListDataObjectBase Ends */ 

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	bool TrySetStringValueFromDisplayString(const FString& InStringValue);

	FString CurrentStringValue;
	FText CurrentDisplayText;
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;
};

UCLASS()
class FPSTEMPLATE_API UListDataObjectStringBool : public UListDataObjectString
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);

	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:
	/* UListDataObjectString */
	virtual void OnDataObjectInitialized() override;

private:
	void TryInitBoolValues(); 

	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

UCLASS()
class FPSTEMPLATE_API UListDataObjectStringEnum : public UListDataObjectString
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(static_cast<int64>(InEnumOption));

		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(static_cast<int64>(InEnumOption));

		SetDefaultValueFromString(ConvertedEnumString);
	}
};

UCLASS()
class FPSTEMPLATE_API UListDataObjectStringInteger : public UListDataObjectString
{
	GENERATED_BODY()

public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText);

protected:
	/* UListDataObjectString */
	virtual void OnDataObjectInitialized() override;

	/* UListDataObjectBase */
	virtual void OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason) override;
};