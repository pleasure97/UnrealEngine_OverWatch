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

	const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FText GetCurrentDisplayText() const { return CurrentDisplayText; }

	/* UListDataObjectBase */
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

protected:
	/* UListDataObjectBase Begins */
	virtual void OnDataObjectInitialized() override;
	/* UListDataObjectBase Ends */ 

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);

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