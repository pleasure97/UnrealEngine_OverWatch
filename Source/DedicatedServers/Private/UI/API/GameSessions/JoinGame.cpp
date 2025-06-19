// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/API/GameSessions/JoinGame.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UJoinGame::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (Image_LoadingThrobber)
	{
		MID_LoadingThrobber = Image_LoadingThrobber->GetDynamicMaterial(); 
	}

	if (Button_CancelJoinGame)
	{
		Button_CancelJoinGame->OnClicked.AddDynamic(this, &UJoinGame::OnJoiningGameCanceled); 
	}

	RollbackOriginalWidget();
}

void UJoinGame::OnJoiningGameCanceled()
{
	// TODO - Click Cancel Join Game Button even with Finding Active Game Session
	RollbackOriginalWidget();
}

void UJoinGame::RollbackOriginalWidget()
{
	if (SizeBox_FindingGame)
	{
		SizeBox_FindingGame->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Border_BeforeFindingGame)
	{
		Border_BeforeFindingGame->SetVisibility(ESlateVisibility::Visible);
	}

	if (Border_AfterFindingGame)
	{
		Border_AfterFindingGame->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Button_JoinGame)
	{
		Button_JoinGame->SetVisibility(ESlateVisibility::Visible);
	}
}

void UJoinGame::ProcessHTTPStatus(const FString& Message, bool bShouldResetWidgets)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);

	if (Message == HTTPStatusMessages::SearchingGameSession)
	{
		if (SizeBox_FindingGame)
		{
			SizeBox_FindingGame->SetVisibility(ESlateVisibility::Visible); 
		}

		if (Button_JoinGame)
		{
			Button_JoinGame->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}
	else if (Message == HTTPStatusMessages::FindActiveGameSession)
	{
		if (Border_BeforeFindingGame)
		{
			Border_BeforeFindingGame->SetVisibility(ESlateVisibility::Hidden);
		}

		if (Border_AfterFindingGame)
		{
			Border_AfterFindingGame->SetVisibility(ESlateVisibility::Visible);
		}

		if (TextBlock_MainDescription)
		{
			TextBlock_MainDescription->SetText(FText::FromString(FindActiveGameSessionString));
		}

		if (TextBlock_SubDescription)
		{
			TextBlock_SubDescription->SetText(FText::FromString(ConnectingToServerString));
		}
	}
	else if (Message == HTTPStatusMessages::SomethingWentWrong)
	{
		if (Button_JoinGame)
		{
			Button_JoinGame->SetIsEnabled(true); 
		}
		RollbackOriginalWidget(); 
	}
}
