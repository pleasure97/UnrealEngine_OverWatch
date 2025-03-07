// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/OWPlayerState.h"
#include "Player/OWPlayerController.h"
#include "UI/HUD/OWHUD.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

AOWCharacter::AOWCharacter()
{
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

	if (AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->OnRep_SelectedHeroName(); 
	}

	InitAbilityActorInfo(); 
	AddHeroAbilities(); 

	InitializeDefaultAttributes();
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

int32 AOWCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState); 
	return OWPlayerState->LevelUpInfo->FindLevelForXP(InXP); 
}

int32 AOWCharacter::GetXP_Implementation() const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	return OWPlayerState->GetXP(); 
}

int32 AOWCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	return OWPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 AOWCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	return OWPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

void AOWCharacter::AddToXP_Implementation(int32 InXP)
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	OWPlayerState->AddToXP(InXP); 
}

void AOWCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	OWPlayerState->AddToLevel(InPlayerLevel); 

	// TODO - Update Eligible Ability if Level is Valid 
	/*if (UOWAbilitySystemComponent* OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(GetAbilitySystemComponent())) */
}

void AOWCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	OWPlayerState->AddToAttributePoints(InAttributePoints); 
}

int32 AOWCharacter::GetAttributePoints_Implementation() const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	return OWPlayerState->GetAttributePoints(); 
}

void AOWCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	OWPlayerState->AddToSpellPoints(InSpellPoints); 
}

int32 AOWCharacter::GetSpellPoints_Implementation() const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	return OWPlayerState->GetSpellPoints(); 
}

void AOWCharacter::LevelUp_Implementation()
{
	MulticastLevelUp(); 
}

int32 AOWCharacter::GetCharacterLevel_Implementation() const
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>();
	check(OWPlayerState);
	return OWPlayerState->GetPlayerLevel();
}

void AOWCharacter::InitAbilityActorInfo()
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>(); 
	check(OWPlayerState); 
	OWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OWPlayerState, this); 
	Cast<UOWAbilitySystemComponent>(OWPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet(); 
	AbilitySystemComponent = OWPlayerState->GetAbilitySystemComponent(); 
	AttributeSet = OWPlayerState->GetAttributeSet(); 

	OnASCRegistered.Broadcast(AbilitySystemComponent); 
	AbilitySystemComponent->RegisterGameplayTagEvent(
		FOWGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AOWCharacter::StunTagChanged); 

	if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetController()))
	{
		if (AOWHUD* OWHUD = Cast<AOWHUD>(OWPlayerController->GetHUD()))
		{
			OWHUD->InitOverlay(OWPlayerController, OWPlayerState, AbilitySystemComponent, AttributeSet); 
		}
	}
}

void AOWCharacter::MulticastLevelUp_Implementation() const
{
	// TODO - Activate Level Up Niagara Component
}
