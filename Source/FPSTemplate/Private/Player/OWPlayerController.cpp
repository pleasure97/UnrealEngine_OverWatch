// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/OWInputComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"

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

void AOWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); 

	UOWInputComponent* OWInputComponent = CastChecked<UOWInputComponent>(InputComponent); 
	OWInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Move); 
	OWInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AOWPlayerController::ShiftPressed); 
	OWInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AOWPlayerController::ShiftReleased); 

	OWInputComponent->BindAbilityActions(InputConfig, this, 
		&AOWPlayerController::AbilityInputTagPressed, &AOWPlayerController::AbilityInputTagReleased, &AOWPlayerController::AbilityInputTagReleased);
}

UOWAbilitySystemComponent* AOWPlayerController::GetAbilitySystemComponent()
{
	if (!OWAbilitySystemComponent)
	{
		OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return OWAbilitySystemComponent;
}

void AOWPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Input_Pressed)) return; 

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); 
	const FRotator Rotation = GetControlRotation(); 
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); 

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); 

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y); 
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X); 
	}
}

void AOWPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Input_Pressed)) return; 
}

void AOWPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Input_Released)) return;
}

void AOWPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FOWGameplayTags::Get().Player_Input_Held)) return;
}

