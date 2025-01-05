// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerController.h"
#include "EnhancedInputSubsystems.h"

AOWPlayerController::AOWPlayerController()
{
	bReplicates = true; 
}

void AOWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(OWContext); 

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); 
	check(Subsystem); 
	Subsystem->AddMappingContext(OWContext, 0); 	
}
