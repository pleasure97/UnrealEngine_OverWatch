// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerStart.h"
#include "GameFramework/GameModeBase.h"

EOWPlayerStartOccupancy AOWPlayerStart::GetLocationOccupancy(AController* const ControllerPawnToFit) const
{
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
