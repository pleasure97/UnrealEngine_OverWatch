// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/Components/OWCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "UI/CommonUI/Util/CommonUISubsystem.h"

void UOWCommonButtonBase::SetButtonDisplayText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText); 
	}
}

void UOWCommonButtonBase::SetButtonDescriptionText(FText InText)
{
	if (CommonTextBlock_ButtonDescriptionText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonDescriptionText->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

void UOWCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	SetButtonDisplayText(ButtonDisplayText); 

	SetButtonDescriptionText(ButtonDescriptionText);
}

void UOWCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged(); 

	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UOWCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionTextWhenHovered.IsEmpty())
	{
		UCommonUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionTextWhenHovered);
	}
}

void UOWCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (!ButtonDescriptionTextWhenHovered.IsEmpty())
	{
		UCommonUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
}
