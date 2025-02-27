// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OWGameModeBase.h"
#include "Game/OWGameState.h"
#include "Character/OWCharacter.h"
#include "Player/OWPlayerState.h"

void AOWGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 

	ChangeHero(GetWorld()->GetFirstPlayerController(), EHeroName::ILLIARI);
}

void AOWGameModeBase::ChangeHero(APlayerController* PlayerController, EHeroName NewHeroName)
{
	if (!PlayerController) return; 

	AOWCharacter* CurrentHero = Cast<AOWCharacter>(PlayerController->GetPawn()); 

	const FVector CurrentHeroLocation = CurrentHero->GetActorLocation(); 
	const FRotator CurrentHeroRotation = CurrentHero->GetActorRotation(); 

	if (CurrentHero)
	{
		CurrentHero->Destroy(); 
	}

	TSubclassOf<AOWCharacter>* NewHeroClass = HeroMap.Find(NewHeroName); 
	if (NewHeroClass && *NewHeroClass)
	{
		FActorSpawnParameters SpawnParams; 
		SpawnParams.Owner = PlayerController; 
		SpawnParams.Instigator = PlayerController->GetPawn(); 

		AOWCharacter* NewCharacter = GetWorld()->SpawnActor<AOWCharacter>(*NewHeroClass, 
			CurrentHeroLocation, CurrentHeroRotation, SpawnParams); 
		if (NewCharacter)
		{
			PlayerController->Possess(NewCharacter); 
		}
	}

	if (AOWPlayerState* OWPlayerState = PlayerController->GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->SetSelectedHeroName(NewHeroName);
	}
}
