// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerStart.h"
#include "Team/OWTeamSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"

bool AOWPlayerStart::IsMatchingTeam(AController* Controller) const
{
	if (!Controller || !Controller->PlayerState)
	{
		return false;
	}

	UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>();

	if (!ensure(TeamSubsystem))
	{
		return false; 
	}

	const int32 ControllerTeamID = TeamSubsystem->FindTeamFromObject(Controller->PlayerState);

	return (ControllerTeamID != INDEX_NONE) && (ControllerTeamID == TeamID); 
}

EOWPlayerStartOccupancy AOWPlayerStart::GetLocationOccupancy(AController* const ControllerPawnToFit) const
{
	if (!IsMatchingTeam(ControllerPawnToFit))
	{
		// If the team doesn't fit, the Player Start's processed as Full.
		return EOWPlayerStartOccupancy::Full;
	}

	UWorld* World = GetWorld(); 
	if (HasAuthority() && World)
	{
		if (AGameModeBase* AuthGameMode = World->GetAuthGameMode())
		{
			TSubclassOf<APawn> PawnClass = AuthGameMode->GetDefaultPawnClassForController(ControllerPawnToFit); 
			const APawn* PawnToFit = PawnClass ? GetDefault<APawn>(PawnClass) : nullptr; 

			FVector ActorLocation = GetActorLocation(); 
			const FRotator ActorRotation = GetActorRotation(); 

			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation, nullptr))
			{
				return EOWPlayerStartOccupancy::Empty; 
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				return EOWPlayerStartOccupancy::Partial; 
			}
		}
	}

	return EOWPlayerStartOccupancy::Full; 
}

bool AOWPlayerStart::IsClaimed() const
{
	return ClaimingController != nullptr; 
}

bool AOWPlayerStart::TryClaim(AController* OccupyingController)
{
	if (OccupyingController != nullptr && !IsClaimed())
	{
		ClaimingController = OccupyingController; 
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ExpirationTimerHandle, 
				FTimerDelegate::CreateUObject(this, &AOWPlayerStart::CheckUnclaimed), ExpirationCheckInterval, true); 
		}
		return true; 
	}
	return false;
}

void AOWPlayerStart::CheckUnclaimed()
{
	if ((ClaimingController != nullptr) 
		&& (ClaimingController->GetPawn() != nullptr) 
		&& GetLocationOccupancy(ClaimingController) == EOWPlayerStartOccupancy::Empty)
	{
		ClaimingController = nullptr; 
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ExpirationTimerHandle);
		}
	}
}
