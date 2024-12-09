// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalHUD.h"
#include "UI/Portal/SignIn/SignInOverlay.h"

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
	OwningPlayerController->SetInputMode(InputModeData); 
	OwningPlayerController->SetShowMouseCursor(true); 
}
