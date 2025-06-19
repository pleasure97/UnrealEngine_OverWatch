// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalHUD.h"
#include "UI/Portal/SignIn/SignInOverlay.h"
#include "UI/Portal/Dashboard/DashboardOverlay.h"

void APortalHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwningPlayerController = GetOwningPlayerController();
	SignInOverlay = CreateWidget<USignInOverlay>(OwningPlayerController, SignInOverlayClass);

	if (IsValid(SignInOverlay))
	{
		SignInOverlay->AddToViewport(); 
	}

	FInputModeGameAndUI InputModeData; 
	if (OwningPlayerController)
	{
		OwningPlayerController->SetInputMode(InputModeData);
		OwningPlayerController->SetShowMouseCursor(true);
	}
}

void APortalHUD::OnSignIn()
{
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->RemoveFromParent(); 
	}
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	DashboardOverlay = CreateWidget<UDashboardOverlay>(OwningPlayerController, DashboardOverlayClass); 
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->AddToViewport(); 
		if (OwningPlayerController)
		{
			// Set Input Mode UI Only 
			FInputModeUIOnly InputModeData;
			// Do not Lock Mouse
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); 
			// Focus on Dashboard Overlay 
			InputModeData.SetWidgetToFocus(DashboardOverlay->TakeWidget()); 

			OwningPlayerController->SetInputMode(InputModeData); 
		}
	}
}

void APortalHUD::OnSignOut()
{
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->RemoveFromParent(); 
	}

	APlayerController* OwningPlayerController = GetOwningPlayerController();
	SignInOverlay = CreateWidget<USignInOverlay>(OwningPlayerController, SignInOverlayClass);

	if (IsValid(SignInOverlay))
	{
		SignInOverlay->AddToViewport();
	}
}
