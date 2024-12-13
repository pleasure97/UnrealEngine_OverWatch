// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UConfirmSignUpPage::ClearTextBoxes()
{
	TextBox_ConfirmationCode->SetText(FText::GetEmpty()); 
	TextBlock_StatusMessage->SetText(FText::GetEmpty()); 
	TextBlock_Destination->SetText(FText::GetEmpty()); 
}

void UConfirmSignUpPage::UpdateStatusMessage(const FString& Message, bool bShouldResetWidget)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message)); 
	if (bShouldResetWidget)
	{
		Button_Confirm->SetIsEnabled(true); 
	}
}
