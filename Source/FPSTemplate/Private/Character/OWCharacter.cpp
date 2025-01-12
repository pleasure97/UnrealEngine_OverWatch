// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacter.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Player/OWPlayerController.h"
#include "UI/HUD/OWHUD.h"

AOWCharacter::AOWCharacter()
{
	/*AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	OWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OWPlayerState, this);
	AbilitySystemComponent = OWPlayerState->GetAbilitySystemComponent();
	AttributeSet = OWPlayerState->GetAttributeSet();

	if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetController()))
	{
		if (AOWHUD* OWHUD = Cast<AOWHUD>(OWPlayerController->GetHUD()))
		{
			OWHUD->InitOverlay(OWPlayerController, OWPlayerState, AbilitySystemComponent, AttributeSet); 
		}
	}*/
}

void AOWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 

	InitAbilityActorInfo(); 
}

void AOWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	// Init ability actor info for the client. 
	InitAbilityActorInfo(); 
}

void AOWCharacter::InitAbilityActorInfo()
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>(); 
	check(OWPlayerState); 
	OWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OWPlayerState, this); 
	AbilitySystemComponent = OWPlayerState->GetAbilitySystemComponent(); 
	AttributeSet = OWPlayerState->GetAttributeSet(); 
}
