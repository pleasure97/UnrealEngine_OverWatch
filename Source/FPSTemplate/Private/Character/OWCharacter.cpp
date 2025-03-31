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
#include "AbilitySystem/OWAttributeSet.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

AOWCharacter::AOWCharacter()
{
	PrimaryActorTick.bCanEverTick = true; 

	FirstPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("FirstPersonSpringArm"); 
	FirstPersonSpringArm->SetupAttachment(GetRootComponent()); 
	FirstPersonSpringArm->TargetArmLength = 0.f; 
	FirstPersonSpringArm->bEnableCameraLag = true; 
	FirstPersonSpringArm->CameraLagSpeed = 15.f; 
	FirstPersonSpringArm->bUsePawnControlRotation = true; 

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera"); 
	FirstPersonCamera->SetupAttachment(FirstPersonSpringArm); 
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

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("ThirdPersonSpringArm"); 
	ThirdPersonSpringArm->SetupAttachment(GetRootComponent()); 

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera"); 
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm); 
	ThirdPersonCamera->SetRelativeLocation(FVector(0., 10., 53.)); 
	ThirdPersonCamera->bUsePawnControlRotation = true; 

	TPtoFPCameraTimeline = CreateDefaultSubobject<UTimelineComponent>("TPtoFPCameraTimeline"); 
	FPtoTPCameraTimeline = CreateDefaultSubobject<UTimelineComponent>("FPtoTPCameraTimeline"); 
}

void AOWCharacter::BeginPlay()
{
	Super::BeginPlay(); 

	FirstPersonCameraTransform = FirstPersonCamera->GetRelativeTransform(); 
	ThirdPersonCameraTransform = ThirdPersonCamera->GetRelativeTransform(); 

	ActiveFirstPersonCamera(); 

	if (FPtoTPCameraTimeline)
	{
		FPtoTPCameraTimeline->SetPlayRate(CameraTransitionSpeed);
	}
	if (TPtoFPCameraTimeline)
	{
		TPtoFPCameraTimeline->SetPlayRate(CameraTransitionSpeed);
	}
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

	if (UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(AttributeSet))
	{
		for (TPair<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& Pair : OWAttributeSet->TagsToAttributes)
		{
			FOnAttributeChangedSignature* AttributeDelegate = GetDelegateForTag(Pair.Key);
			BindAttributeChange(AbilitySystemComponent, Pair.Key, Pair.Value(), *AttributeDelegate);
		}

		OnMaxHealthChanged.Broadcast(OWAttributeSet->GetMaxHealth());
		OnMaxArmorChanged.Broadcast(OWAttributeSet->GetMaxArmor());
		OnMaxShieldChanged.Broadcast(OWAttributeSet->GetMaxShield());
		OnHealthChanged.Broadcast(OWAttributeSet->GetHealth());
		OnArmorChanged.Broadcast(OWAttributeSet->GetArmor());
		OnShieldChanged.Broadcast(OWAttributeSet->GetShield());
		OnTempArmorChanged.Broadcast(OWAttributeSet->GetTempArmor());
		OnTempShieldChanged.Broadcast(OWAttributeSet->GetTempShield());
	}
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

void AOWCharacter::TransitionCamera_Implementation(bool bSmoothTransition)
{
	if (bCameraTransitioning) return; 

	bCameraTransitioning = true; 

	if (bSmoothTransition)
	{
		if (bFirstPersonView)
		{
			if (!SlideAmount || !FPtoTPCameraTimeline) return; 

			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindUFunction(this, FName("UpdateFPtoTPCamera"));
			FPtoTPCameraTimeline->AddInterpFloat(SlideAmount, TimelineProgress);

			FOnTimelineEvent TimelineFinished;
			TimelineFinished.BindUFunction(this, FName("FinishFPtoTPCamera"));
			FPtoTPCameraTimeline->SetTimelineFinishedFunc(TimelineFinished);

			FPtoTPCameraTimeline->SetLooping(false);
			FPtoTPCameraTimeline->PlayFromStart();

			if (IsLocallyControlled() || HasAuthority())
			{
				GetMesh()->SetVisibility(true, true);
				FirstPersonMesh->SetVisibility(false, true);
			}
		}
		else
		{
			if (!SlideAmount || !TPtoFPCameraTimeline) return; 

			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindUFunction(this, FName("UpdateTPtoFPCamera"));
			TPtoFPCameraTimeline->AddInterpFloat(SlideAmount, TimelineProgress);

			FOnTimelineEvent TimelineFinished;
			TimelineFinished.BindUFunction(this, FName("FinishTPtoFPCamera"));
			TPtoFPCameraTimeline->SetTimelineFinishedFunc(TimelineFinished);

			TPtoFPCameraTimeline->SetLooping(false);
			TPtoFPCameraTimeline->PlayFromStart();

			if (IsLocallyControlled() || HasAuthority())
			{
				GetMesh()->SetVisibility(false, true);
				FirstPersonMesh->SetVisibility(true, true);
			}
		}
	}
	else
	{
		if (bFirstPersonView)
		{
			ActiveThirdPersonCamera(); 
		}
		else
		{
			ActiveFirstPersonCamera(); 
		}
	}
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

void AOWCharacter::ActiveFirstPersonCamera()
{
	bUseControllerRotationYaw = true; 
	ThirdPersonCamera->SetActive(false); 
	FirstPersonCamera->SetActive(true); 
	bFirstPersonView = true; 
}

void AOWCharacter::ActiveThirdPersonCamera()
{
	bUseControllerRotationYaw = false; 
	FirstPersonCamera->SetActive(false); 
	ThirdPersonCamera->SetActive(true); 
	bFirstPersonView = false; 
}

void AOWCharacter::UpdateFPtoTPCamera(float Output)
{
	FTransform NewTransform = UKismetMathLibrary::TLerp(FirstPersonCamera->GetComponentTransform(), ThirdPersonCamera->GetComponentTransform(), Output); 
	FirstPersonCamera->SetWorldTransform(NewTransform); 
}

void AOWCharacter::FinishFPtoTPCamera()
{
	ActiveThirdPersonCamera(); 
	FirstPersonCamera->SetRelativeTransform(FirstPersonCameraTransform); 
	bCameraTransitioning = false; 
}

void AOWCharacter::UpdateTPtoFPCamera(float Output)
{
	FVector NewVector = UKismetMathLibrary::VLerp(ThirdPersonCamera->GetComponentLocation(), FirstPersonCamera->GetComponentLocation(), Output);
	ThirdPersonCamera->SetWorldLocation(NewVector); 

	FRotator NewRotator = UKismetMathLibrary::RLerp(GetCapsuleComponent()->GetComponentRotation(), ThirdPersonCamera->GetComponentRotation(), Output, false); 
	GetCapsuleComponent()->SetWorldRotation(NewRotator); 
}

void AOWCharacter::FinishTPtoFPCamera()
{
	ActiveFirstPersonCamera(); 
	ThirdPersonCamera->SetRelativeTransform(ThirdPersonCameraTransform); 
	bCameraTransitioning = false;
}

void AOWCharacter::MulticastLevelUp_Implementation() const
{
	// TODO - Activate Level Up Niagara Component
}
