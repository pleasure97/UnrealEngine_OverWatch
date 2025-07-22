// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthPlate.h"
#include "Components/TextBlock.h"
#include "UI/Widget/PlayerHealthBarPool.h"
#include "Player/OWPlayerState.h"
#include "Team/OWTeamSubsystem.h"

void UHealthPlate::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 
}

void UHealthPlate::NativeDestruct()
{
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

		OWPlayerState->GetTeamChangedDelegate().AddDynamic(this, &UHealthPlate::OnTeamChanged);
		if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
		{
			int32 TeamID = TeamSubsystem->FindTeamFromObject(OWPlayerState);
			if (TeamID == 1 || TeamID == 2)
			{
				OnTeamChanged(OWPlayerState, -1, TeamID);
			}
		}

		UpdatePlayerName();

		UpdatePlayerHealthBarPool();
	}
}

void UHealthPlate::OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID)
{

}

void UHealthPlate::UpdatePlayerHealthBarPool()
{
	if (WBP_PlayerHealthBarPool)
	{
		WBP_PlayerHealthBarPool->SetPlayerState(OWPlayerState);
	}
}

void UHealthPlate::UpdatePlayerName()
{
	if (OWPlayerState)
	{
		if (TextBlock_PlayerName)
		{
			FString OwnerPlayerName = OWPlayerState->GetPlayerName();
			if (OwnerPlayerName.Len() > 12)
			{
				OwnerPlayerName = OwnerPlayerName.Right(12);
			}
			TextBlock_PlayerName->SetText(FText::FromString(OwnerPlayerName));
		}

	}
}