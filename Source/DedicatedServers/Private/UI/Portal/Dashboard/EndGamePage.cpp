// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/EndGamePage.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndGamePage::NativeConstruct()
{
	Super::NativeConstruct();

	Button_CancelEndGame->OnClicked.AddDynamic(this, &UEndGamePage::OnCancelEndGameButtonClicked); 
	Button_EndGame->OnClicked.AddDynamic(this, &UEndGamePage::OnEndGameButtonClicked); 
}

void UEndGamePage::OnCancelEndGameButtonClicked()
{
	// NOTE - End Game Page's Parent Must be Widget Switcher, and Set Widget Switcher's Index 0 Should be Main Widget.  
	if (UWidgetSwitcher* WidgetSwitcher = Cast<UWidgetSwitcher>(GetParent()))
	{
		check(WidgetSwitcher); 
		WidgetSwitcher->SetActiveWidgetIndex(0); 
	}
}

void UEndGamePage::OnEndGameButtonClicked()
{
	// TODO - Need for Further Processing when Participating in Game Session?
	// Logic for Gameplay Statics 
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		nullptr,	// Get Player Controller internally using Gameplay Statics
		EQuitPreference::Quit,
		true);
}


