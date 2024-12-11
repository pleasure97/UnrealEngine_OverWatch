// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"
#include "UI/Portal/PortalManager.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "UI/Portal/SignIn/SuccessConfirmedPage.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(PortalManagerClass); 
	check(IsValid(JoinGameWidget)); 
	check(IsValid(JoinGameWidget->Button_JoinGame)); 

	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass); 

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked); 

	check(Button_SignIn_Test); 
	check(Button_SignUp_Test); 
	check(Button_ConfirmSignUp_Test); 
	check(Button_SuccessConfirmed_Test); 

	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage); 
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage); 
	Button_ConfirmSignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmSignUpPage); 
	Button_SuccessConfirmed_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmedPage); 
}

void USignInOverlay::OnJoinGameButtonClicked()
{
	check(IsValid(PortalManager));
	check(IsValid(JoinGameWidget));
	check(IsValid(JoinGameWidget->Button_JoinGame));

	PortalManager->BroadcastJoinGameSessionMessage.AddDynamic(this, &USignInOverlay::UpdateJoinGameStatus); 
	PortalManager->JoinGameSession();	
	JoinGameWidget->Button_JoinGame->SetIsEnabled(false); 
}

void USignInOverlay::UpdateJoinGameStatus(const FString& StatusMessage, bool bShouldResetJoinGameButton)
{
	check(IsValid(JoinGameWidget));
	check(IsValid(JoinGameWidget->Button_JoinGame));
	JoinGameWidget->SetStatusMessage(StatusMessage); 

	if (bShouldResetJoinGameButton)
	{
		JoinGameWidget->Button_JoinGame->SetIsEnabled(true);	
	}
}

void USignInOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage)); 
	WidgetSwitcher->SetActiveWidget(SignInPage); 
}

void USignInOverlay::ShowSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignUpPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(ConfirmSignUpPage));
	WidgetSwitcher->SetActiveWidget(ConfirmSignUpPage);
}

void USignInOverlay::ShowSuccessConfirmedPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SuccessConfirmedPage));
	WidgetSwitcher->SetActiveWidget(SuccessConfirmedPage); 
}
