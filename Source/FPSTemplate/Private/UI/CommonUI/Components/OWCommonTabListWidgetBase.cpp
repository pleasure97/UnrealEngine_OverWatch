// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Components/OWCommonTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "UI/CommonUI/Components/OWCommonButtonBase.h"

void UOWCommonTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr /* Content Widget */);

	if (UOWCommonButtonBase* FoundButton = Cast<UOWCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonDisplayText(InTabDisplayName);
	}
}
#if WITH_EDITOR
void UOWCommonTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(TEXT("TabButtonEntryWidgetClass is Not Valid in UOWCommonTabListWidgetBase::ValidateCompiledDefaults()")));
	}
}
#endif