// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
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

	PrimaryActorTick.bCanEverTick = true; 

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm"); 
	SpringArm->SetupAttachment(GetRootComponent()); 
	SpringArm->TargetArmLength = 0.f; 
	SpringArm->bEnableCameraLag = true; 
	SpringArm->CameraLagSpeed = 15.f; 
	SpringArm->bUsePawnControlRotation = true; 

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera"); 
	FirstPersonCamera->SetupAttachment(SpringArm); 
	FirstPersonCamera->bUsePawnControlRotation = false;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh"); 
	FirstPersonMesh->SetupAttachment(FirstPersonCamera); 
	// Show FirstPersonMesh visible to the user, invisible to other clients 
	FirstPersonMesh->bOnlyOwnerSee = true; 
	FirstPersonMesh->bOwnerNoSee = false; 
	FirstPersonMesh->bCastDynamicShadow = false; 
	FirstPersonMesh->bReceivesDecals = false; 
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered; 
	FirstPersonMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics; 

	// Show ThirdPersonMesh visible to other clients, invisible to the user 
	GetMesh()->bOnlyOwnerSee = false; 
	GetMesh()->bOwnerNoSee = true; 
	GetMesh()->bReceivesDecals = false; 
}

void AOWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 

	InitAbilityActorInfo(); 
}

void AOWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 

	UEnhancedInputComponent* OWInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent); 

}

void AOWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	// Init ability actor info for the client. 
	InitAbilityActorInfo(); 
}

void AOWCharacter::BeginPlay()
{
	Super::BeginPlay(); 

	if (AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OWPlayerState, this);
		AbilitySystemComponent = OWPlayerState->GetAbilitySystemComponent();
		AttributeSet = OWPlayerState->GetAttributeSet();

		if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetController()))
		{
			if (AOWHUD* OWHUD = Cast<AOWHUD>(OWPlayerController->GetHUD()))
			{
				OWHUD->InitOverlay(OWPlayerController, OWPlayerState, AbilitySystemComponent, AttributeSet);
			}
		}
	}
}

void AOWCharacter::InitAbilityActorInfo()
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>(); 
	check(OWPlayerState); 
	OWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OWPlayerState, this); 
	AbilitySystemComponent = OWPlayerState->GetAbilitySystemComponent(); 
	AttributeSet = OWPlayerState->GetAttributeSet(); 
}
