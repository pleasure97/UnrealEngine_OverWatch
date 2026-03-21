// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CommonUI/Options/ListDataObjectBase.h"
#include "ListDataObjectValue.generated.h"

class FOptionsDataInteractionHelper;

/**
 * 
 */
UCLASS(Abstract)
class FPSTEMPLATE_API UListDataObjectValue : public UListDataObjectBase
{
	GENERATED_BODY()
	
public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultStringValue) { DefaultStringValue = InDefaultStringValue; }

	/* UListDataObjectBase */
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }

protected:
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }

	// Use Shared Pointer Because "FOptionsDataInteractionHelper" is Pure C++ Class
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue; 
};
