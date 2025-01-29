// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/OWInputComponent.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OWGameplayTags.h"
#include "Interfaces/PlayerInterface.h"

AOWPlayerController::AOWPlayerController()
{
	bReplicates = true; 
	bPlayerAlive = true; 
}

void AOWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 

	bPlayerAlive = true; 
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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent); 
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Input_Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOWPlayerController::Input_Look);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AOWPlayerController::Input_Crouch);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AOWPlayerController::Input_Jump);

	//OWInputComponent->BindAbilityActions(InputConfig, this, 
	//	&AOWPlayerController::AbilityInputTagPressed, &AOWPlayerController::AbilityInputTagReleased, &AOWPlayerController::AbilityInputTagReleased);
}

UOWAbilitySystemComponent* AOWPlayerController::GetAbilitySystemComponent()
{
	if (!OWAbilitySystemComponent)
	{
		OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return OWAbilitySystemComponent;
}

void AOWPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!bPlayerAlive) return;

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

void AOWPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); 
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}

void AOWPlayerController::Input_Crouch()
{
	if (!bPlayerAlive) return; 
	if (GetPawn() == nullptr || !GetPawn()->Implements<UPlayerInterface>()) return; 
	IPlayerInterface::Execute_Initiate_Crouch(GetPawn()); 
}

void AOWPlayerController::Input_Jump()
{
	if (!bPlayerAlive) return; 
	if (GetPawn() == nullptr || !GetPawn()->Implements<UPlayerInterface>()) return;
	IPlayerInterface::Execute_Initiate_Jump(GetPawn());
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

