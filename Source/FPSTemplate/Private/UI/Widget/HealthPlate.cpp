// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthPlate.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/Widget/PlayerHealthBarPool.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"

void UHealthPlate::NativeDestruct()
{
	// Remove Team Changed Delegate Binding of Player State
	if (OWPlayerState)
	{
		OWPlayerState->GetTeamChangedDelegate().RemoveAll(this);
	}

	Super::NativeDestruct(); 
}

void UHealthPlate::SetPlayerState(AOWPlayerState* InOWPlayerState)
{
	if ((OWPlayerState == nullptr) && (InOWPlayerState != nullptr))
	{
		OWPlayerState = InOWPlayerState;

		// Set Player State of Player Health Bar Pool 
		if (WBP_PlayerHealthBarPool)
		{
			WBP_PlayerHealthBarPool->SetPlayerState(OWPlayerState);
		}

		// Bind Team Changed Delegate of Player State 
		OWPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UHealthPlate::OnTeamChanged);
		if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
		{
			// If Team ID is already set, then Call OnTeamChanged()
			int32 TeamID = TeamSubsystem->FindTeamFromObject(OWPlayerState);
			if (TeamID == 1 || TeamID == 2)
			{
				OnTeamChanged(OWPlayerState, -1, TeamID);
			}
		}
	}
}

AOWPlayerState* UHealthPlate::GetOWPlayerState() const
{
	return OWPlayerState; 
}

void UHealthPlate::OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID)
{
	// Check if Owner Player State is set 
	if (OWPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Owning Player State is yet to set in UHealthPlate::OnTeamChanged()"));
		return;
	}

	// New Team ID should be only 1 or 2 
	if ((NewTeamID != 1) && (NewTeamID != 2))
	{
		UE_LOG(LogTemp, Error, TEXT("Team ID is not 0 or 1 in UHealthPlate::OnTeamChanged()"));
		return;
	}
	
	// Get Owning Player Controller
	if (APlayerController* LocalPlayerController = GetOwningPlayer())
	{
		// Get Owning Local Player State 
		AOWPlayerState* LocalPlayerState = LocalPlayerController ? Cast<AOWPlayerState>(LocalPlayerController->PlayerState) : nullptr; 

		// Set My Health Plate Visibility to be Collapsed 
		if (LocalPlayerState == OWPlayerState)
		{
			SetVisibility(ESlateVisibility::Collapsed); 
			return;
		}
	
		// Check if Local Player State is on Same Team 
		const bool bAlly = (LocalPlayerState->GetTeamId() == OWPlayerState->GetTeamId());
		UpdatePlayerInfo(bAlly);
	}
}

void UHealthPlate::UpdatePlayerInfo(bool bAlly)
{
	if (OWPlayerState)
	{
		UpdatePlayerName(bAlly);
		UpdatePlayerArrow(bAlly);
		UpdatePlayerHealthBarPool(bAlly); 
	}
}

void UHealthPlate::UpdatePlayerName(bool bAlly)
{
	if (TextBlock_PlayerName)
	{
		if (bAlly)
		{
			TextBlock_PlayerName->SetColorAndOpacity(FSlateColor(AllyColor));
		}
		else
		{
			TextBlock_PlayerName->SetColorAndOpacity(FSlateColor(EnemyColor));
		}
		FString OwnerPlayerName = OWPlayerState->GetPlayerName();
		if (OwnerPlayerName.Len() > 12)
		{
			OwnerPlayerName = OwnerPlayerName.Right(12);
		}
		TextBlock_PlayerName->SetText(FText::FromString(OwnerPlayerName));
	}
}

void UHealthPlate::UpdatePlayerArrow(bool bAlly)
{
	if (!bAlly)
	{
		if (Image_PlayerArrow)
		{
			Image_PlayerArrow->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}
}

void UHealthPlate::UpdatePlayerHealthBarPool(bool bAlly)
{
	if (!bAlly)
	{
		if (WBP_PlayerHealthBarPool)
		{
			WBP_PlayerHealthBarPool->SetIsEnemy(true); 
		}
	}
}