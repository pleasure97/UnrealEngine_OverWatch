// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI/OWCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "UI/CommonUI/CommonUISubsystem.h"

void UOWCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText); 
	}
}

void UOWCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	SetButtonText(ButtonDisplayText); 
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

	if (!ButtonDescriptionText.IsEmpty())
	{
		UCommonUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UOWCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UCommonUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
}
