// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Options/ListDataObjectValue.h"

void UListDataObjectValue::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

void UListDataObjectValue::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}
