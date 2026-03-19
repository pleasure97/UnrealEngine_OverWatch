// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/PrimaryLayoutWidget.h"
#include "UI/CommonUI/Util/CommonUIDebugHelper.h"

UCommonActivatableWidgetContainerBase* UPrimaryLayoutWidget::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisterWidgetStackMap.Contains(InTag), TEXT("Cannot Find Widget Stack by Tag %s"), *InTag.ToString()); 

	return RegisterWidgetStackMap.FindRef(InTag);
}

void UPrimaryLayoutWidget::RegisterWidgetStack(UPARAM(meta = (Categories = "CommonUI.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisterWidgetStackMap.Contains(InStackTag))
		{
			RegisterWidgetStackMap.Add(InStackTag, InStack);

			CommonUIDebug::Print(TEXT("Widget Stack Registered Under Tag ") + InStackTag.ToString());
		}
	}
}
