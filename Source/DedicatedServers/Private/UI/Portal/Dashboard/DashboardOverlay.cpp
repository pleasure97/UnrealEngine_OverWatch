// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"
#include "UI/Portal/Dashboard/EndGamePage.h"
#include "UI/GameStats/GameStatsManager.h"
#include "Components/Overlay.h"
#include "Components/Button.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct(); 

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass); 
	GameStatsManager->OnRetrieveMatchStatsResponseReceived.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStats); 
	GameStatsManager->RetrieveMatchStatsStatusMessage.AddDynamic(CareerPage, &UCareerPage::SetStatusMessage); 
	GameStatsManager->OnRetrieveLeaderboard.AddDynamic(LeaderboardPage, &ULeaderboardPage::PopulateLeaderboard); 
	GameStatsManager->RetrieveLeaderboardStatusMessage.AddDynamic(LeaderboardPage, &ULeaderboardPage::SetStatusMessage);

	if (Button_Game)
	{
		Button_Game->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
		Button_Game->OnHovered.AddDynamic(this, &UDashboardOverlay::OnButtonGameHovered); 
		Button_Game->OnUnhovered.AddDynamic(this, &UDashboardOverlay::OnButtonGameUnhovered); 
	}

	if (Button_Career)
	{
		Button_Career->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
		Button_Career->OnHovered.AddDynamic(this, &UDashboardOverlay::OnButtonCareerHovered);
		Button_Career->OnUnhovered.AddDynamic(this, &UDashboardOverlay::OnButtonCareerUnhovered);
	}

	if (Button_Leaderboard)
	{
		Button_Leaderboard->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);
		Button_Leaderboard->OnHovered.AddDynamic(this, &UDashboardOverlay::OnButtonLeaderboardHovered);
		Button_Leaderboard->OnUnhovered.AddDynamic(this, &UDashboardOverlay::OnButtonLeaderboardUnhovered);
	}

	if (Button_Setting)
	{
		Button_Setting->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowSettingPage);
		Button_Setting->OnHovered.AddDynamic(this, &UDashboardOverlay::OnButtonSettingHovered);
		Button_Setting->OnUnhovered.AddDynamic(this, &UDashboardOverlay::OnButtonSettingUnhovered);
	}

	if (Button_EndGame)
	{
		Button_EndGame->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowEndGamePage);
		Button_EndGame->OnHovered.AddDynamic(this, &UDashboardOverlay::OnButtonEndGameHovered);
		Button_EndGame->OnUnhovered.AddDynamic(this, &UDashboardOverlay::OnButtonEndGameUnhovered);
	}

	ShowMainPage(); 
}

bool UDashboardOverlay::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	bIsFocusable = true; 
	return true; 
}

FReply UDashboardOverlay::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// If Escape Key is down and Active Widget is Main Page 
	if ((InKeyEvent.GetKey() == EKeys::Escape) && WidgetSwitcher && (WidgetSwitcher->GetActiveWidgetIndex() == 0))
	{
		ShowEndGamePage(); 
	}
	// If Escape Key is down and Active Widget is not Main Page 
	else if ((InKeyEvent.GetKey() == EKeys::Escape) && WidgetSwitcher && (WidgetSwitcher->GetActiveWidgetIndex() != 0))
	{
		ShowMainPage(); 
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent); 
}

/* Main Page */
void UDashboardOverlay::ShowMainPage()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(Overlay_MainPage);
	}
}
/* Main Page End */

/* Game Page */
void UDashboardOverlay::ShowGamePage()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(GamePage);
	}
}

void UDashboardOverlay::OnButtonGameHovered()
{
	if (GameButtonHovered)
	{
		PlayAnimation(GameButtonHovered, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UDashboardOverlay::OnButtonGameUnhovered()
{
	if (GameButtonHovered)
	{
		PlayAnimation(GameButtonHovered, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}
/* Game Page End */ 

/* Career Page */
void UDashboardOverlay::ShowCareerPage()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(CareerPage);
	}
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::OnButtonCareerHovered()
{
	if (CareerButtonHovered)
	{
		PlayAnimation(CareerButtonHovered, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UDashboardOverlay::OnButtonCareerUnhovered()
{
	if (CareerButtonHovered)
	{
		PlayAnimation(CareerButtonHovered, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}
/* Career Page End */

/* Leaderboard Page */
void UDashboardOverlay::ShowLeaderboardPage()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(LeaderboardPage);
	}
	GameStatsManager->RetrieveLeaderboard();
}

void UDashboardOverlay::OnButtonLeaderboardHovered()
{
	if (LeaderboardButtonHovered)
	{
		PlayAnimation(LeaderboardButtonHovered, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UDashboardOverlay::OnButtonLeaderboardUnhovered()
{
	if (LeaderboardButtonHovered)
	{
		PlayAnimation(LeaderboardButtonHovered, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}
/* Leaderboard Page End */

/* Setting Page */
void UDashboardOverlay::ShowSettingPage()
{

}

void UDashboardOverlay::OnButtonSettingHovered()
{
	if (SettingButtonHovered)
	{
		PlayAnimation(SettingButtonHovered, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UDashboardOverlay::OnButtonSettingUnhovered()
{
	if (SettingButtonHovered)
	{
		PlayAnimation(SettingButtonHovered, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}
/* Setting Page End */

/* EndGame Page */
void UDashboardOverlay::ShowEndGamePage()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(EndGamePage);
	}
}

void UDashboardOverlay::OnButtonEndGameHovered()
{
	if (EndGameButtonHovered)
	{
		PlayAnimation(EndGameButtonHovered, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UDashboardOverlay::OnButtonEndGameUnhovered()
{
	if (EndGameButtonHovered)
	{
		PlayAnimation(EndGameButtonHovered, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}
/* EndGame Page End */