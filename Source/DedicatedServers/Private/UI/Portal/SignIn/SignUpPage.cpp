// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct(); 

	TextBox_UserName->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState); 
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState); 
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState); 
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState); 
	Button_SignUp->SetIsEnabled(false); 
}

void USignUpPage::UpdateSignUpButtonState(const FText& Text)
{
	FString StatusMessage;

	const bool bIsUsernameValid = !TextBox_UserName->GetText().ToString().IsEmpty();
	const bool bArePasswordsEqual = TextBox_Password->GetText().ToString() == TextBox_ConfirmPassword->GetText().ToString(); 
	const bool bIsValidEmail = IsValidEmail(TextBox_Email->GetText().ToString()); 
	const bool bIsStrongPassword = IsStrongPassword(TextBox_Password->GetText().ToString(), StatusMessage);
	const bool bIsPasswordLongEnough = TextBox_Password->GetText().ToString().Len() >= 8; 

	if (!bIsStrongPassword)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage)); 
	}
	else if (!bIsUsernameValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid Username."))); 
	}
	else if (!bArePasswordsEqual)
	{ 
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please ensure that passwords match.")));
	}
	else if (!bIsValidEmail)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid Email.")));
	}
	else if (!bIsPasswordLongEnough)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Passwords must be at least 8 characters.")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::GetEmpty()); 
	}

	Button_SignUp->SetIsEnabled(bIsUsernameValid && bArePasswordsEqual && bIsValidEmail && bIsStrongPassword); 
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	const FRegexPattern EmailPattern(TEXT(R"((^[^\s@]+@[^\s@]+\.[^\s@]{2,}$))"));

	FRegexMatcher Matcher(EmailPattern, Email); 
	return Matcher.FindNext(); 
}

bool USignUpPage::IsStrongPassword(const FString& Password, FString& StatusMessage)
{
	const FRegexPattern NumberPattern(TEXT(R"(\d)")); // Contains at least 1 number 
	const FRegexPattern SpecialCharPattern(TEXT(R"([^\w\s])")); // Contains at least 1 special character 
	const FRegexPattern UpperCasePattern(TEXT(R"([A-Z])")); // Contains at least 1 uppercase character
	const FRegexPattern LowerCasePattern(TEXT(R"([a-z])")); // Contains at least 1 lowercase character
	
	FRegexMatcher NumberMatcher(NumberPattern, Password); 
	FRegexMatcher SpecialCharMatcher(SpecialCharPattern, Password); 
	FRegexMatcher UppercaseMatcher(UpperCasePattern, Password); 
	FRegexMatcher LowercaseMatcher(LowerCasePattern, Password); 

	if (!NumberMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least 1 number."); 
		return false; 
	}
	if (!SpecialCharMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least 1 special character."); 
		return false;
	}
	if (!UppercaseMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least 1 uppercase character."); 
		return false;
	}
	if (!LowercaseMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least 1 lowercase character."); 
		return false; 
	}

	return true; 
}
