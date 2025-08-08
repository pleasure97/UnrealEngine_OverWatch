// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/CameraTransitionComponent.h"
#include "Component/ScreenEffectComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HealthPlate.h"
#include "Player/OWPlayerState.h"
#include "Player/OWPlayerController.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/OWAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Game/OWGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Message/OWMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "UI/HUD/OWHUD.h"

AOWCharacter::AOWCharacter()
{
	PrimaryActorTick.bCanEverTick = true; 

	// First Person Spring Arm 
	FirstPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("FirstPersonSpringArm");
	FirstPersonSpringArm->SetupAttachment(GetRootComponent());
	FirstPersonSpringArm->TargetArmLength = 0.f;
	FirstPersonSpringArm->bEnableCameraLag = true;
	FirstPersonSpringArm->CameraLagSpeed = 15.f;
	FirstPersonSpringArm->bUsePawnControlRotation = true;

	// First Person Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->SetupAttachment(FirstPersonSpringArm);
	FirstPersonCamera->bUsePawnControlRotation = false;

	// First Person Mesh 
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->bOnlyOwnerSee = true;
	FirstPersonMesh->bOwnerNoSee = false;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	FirstPersonMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	// Third Person Spring Arm 
	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("ThirdPersonSpringArm"); 
	ThirdPersonSpringArm->SetupAttachment(GetRootComponent());

	// Third Person Camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera"); 
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCamera->SetRelativeLocation(FVector(0., 10., 53.));
	ThirdPersonCamera->bUsePawnControlRotation = true;

	// Camera Transition Component
	CameraTransitionComponent = CreateDefaultSubobject<UCameraTransitionComponent>("CameraTransitionComponent");
	CameraTransitionComponent->FirstPersonSpringArm = FirstPersonSpringArm;
	CameraTransitionComponent->FirstPersonCamera = FirstPersonCamera;
	CameraTransitionComponent->FirstPersonMesh = FirstPersonMesh; 
	CameraTransitionComponent->ThirdPersonSpringArm = ThirdPersonSpringArm; 
	CameraTransitionComponent->ThirdPersonCamera = ThirdPersonCamera;

	// Screen Effect Component
	ScreenEffectComponent = CreateDefaultSubobject<UScreenEffectComponent>("ScreenEffectComponent");

	// Show ThirdPersonMesh visible to other clients, invisible to the user 
	GetMesh()->bOnlyOwnerSee = false; 
	GetMesh()->bOwnerNoSee = true; 
	GetMesh()->bReceivesDecals = false; 
}

void AOWCharacter::BeginPlay()
{
	Super::BeginPlay(); 

	CameraTransitionComponent->SetFirstPersonCameraTransform(FirstPersonCamera->GetRelativeTransform()); 
	CameraTransitionComponent->SetThirdPersonCameraTransform(ThirdPersonCamera->GetRelativeTransform()); 
	CameraTransitionComponent->ActiveFirstPersonCamera(); 

	// Show FirstPersonMesh visible to the user, invisible to other clients 
	if (IsLocallyControlled())
	{
		if (ScreenEffectComponent)
		{
			ScreenEffectComponent->ApplyPostProcessMaterials(FirstPersonCamera);
			ScreenEffectComponent->ApplyPostProcessMaterials(ThirdPersonCamera);
			ScreenEffectComponent->SetScalarParameterValue(TEXT("TeamID"), GenericTeamIdToInteger(MyTeamID));
		}
	}

	InitializeHealthPlate();

	InitializeOverlay();
}

void AOWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 

	if (AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->OnRep_HeroName(); 
	}

	if (ITeamInterface* ControllerWithTeamInterface = Cast<ITeamInterface>(NewController))
	{
		MyTeamID = ControllerWithTeamInterface->GetGenericTeamId();
		ControllerWithTeamInterface->GetTeamChangedDelegate().AddDynamic(this, &AOWCharacter::OnTeamChanged);
	}

	OnRep_PlayerState(); 
}

void AOWCharacter::UnPossessed()
{
	Super::UnPossessed();

	/*if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ITeamInterface* ControllerWithTeamInterface = Cast<ITeamInterface>(PC))
		{
			MyTeamID = ControllerWithTeamInterface->GetGenericTeamId();
			ControllerWithTeamInterface->GetTeamChangedDelegate().RemoveAll(this);
		}
	}
	
	if (AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>())
	{
		if (UAbilitySystemComponent* PlayerStateASC = OWPlayerState->GetAbilitySystemComponent())
		{
			PlayerStateASC->ClearActorInfo(); 
		}
		OWPlayerState->SetHeroName(EHeroName::None); 
	}

	if (AttributeSet)
	{
		AttributeSet->OnDeath.RemoveAll(this); 
	}

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FOWGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).RemoveAll(this); 

		if (HasAuthority())
		{
			UOWAbilitySystemComponent* OWAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(AbilitySystemComponent); 
			if (OWAbilitySystemComponent && OWAbilitySystemComponent->bDefaultAbilitiesGiven)
			{
				OWAbilitySystemComponent->ClearAllAbilities(); 
				OWAbilitySystemComponent->bDefaultAbilitiesGiven = false;
			}
		}
	}*/
}

void AOWCharacter::InitializeDefaultAttributes() const
{
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetPlayerState()))
	{
		EHeroName HeroName = OWPlayerState->GetHeroName(); 
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent(); 

		if ((HeroName != EHeroName::None) && IsValid(ASC))
		{
			UOWAbilitySystemLibrary::InitializeDefaultAttributes(this, HeroName, ASC);
		}
	}
}

void AOWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	if (!bPossessed)
	{
		InitAbilityActorInfo();

		AddHeroAbilities();

		InitializeDefaultAttributes();

		bPossessed = true; 
	}
}

void AOWCharacter::InitializeOverlay()
{
	// Get Player Controller and Cast it to Custom Player Controller
	if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetController()))
	{
		// Get Custom Player State from Custom Player Controller
		if (AOWPlayerState* OWPlayerState = OWPlayerController->GetPlayerState<AOWPlayerState>())
		{
			// Check if Ability System Component and Attribute Set are Valid 
			if (IsValid(AbilitySystemComponent) && IsValid(AttributeSet))
			{
				// Get HUD from Custom Player Controller and Pass Related Values 
				if (AOWHUD* OWHUD = Cast<AOWHUD>(OWPlayerController->GetHUD()))
				{
					OWHUD->InitOverlay(OWPlayerController, OWPlayerState, AbilitySystemComponent, AttributeSet);
				}
			}
		}
	}
}

void AOWCharacter::InitializeHealthPlate()
{
	// Find Widget Component
	if (UWidgetComponent* FoundWidgetComponent = FindComponentByClass<UWidgetComponent>())
	{
		// Get Health Plate from Widget Component
		if (UHealthPlate* HealthPlate = Cast<UHealthPlate>(FoundWidgetComponent->GetUserWidgetObject()))
		{
			// Set Player State of Health Plate After Character is Possessed
			if (AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>())
			{
				HealthPlate->SetPlayerState(OWPlayerState);
			}
		}
	}
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

void AOWCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse); 

	if (FirstPersonMesh)
	{
		FirstPersonMesh->bOnlyOwnerSee = false;
		FirstPersonMesh->bOwnerNoSee = true;
		FirstPersonMesh->MarkRenderStateDirty(); 
	}

	if (GetMesh())
	{
		GetMesh()->bOnlyOwnerSee = false;
		GetMesh()->bOwnerNoSee = false;
		GetMesh()->MarkRenderStateDirty();
	}

	if (CameraTransitionComponent)
	{
		CameraTransitionComponent->ActiveThirdPersonCamera();
	}
}

UAnimInstance* AOWCharacter::GetFirstPersonMeshAnimInstance_Implementation() const
{
	return FirstPersonMesh->GetAnimInstance(); 
}

void AOWCharacter::TransitionCamera_Implementation(bool bSmoothTransition)
{
	if (CameraTransitionComponent)
	{
		CameraTransitionComponent->TransitionCamera(bSmoothTransition);
	}
}

EAttackDirection AOWCharacter::GetAttackDirection_Implementation() const
{
	return AttackDirection;
}

void AOWCharacter::SetAttackDirection_Implementation(EAttackDirection InAttackDirection)
{
	AttackDirection = InAttackDirection; 
}

void AOWCharacter::OnTeamChanged(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	if (IsLocallyControlled())
	{
		ScreenEffectComponent->ApplyPostProcessMaterials(FirstPersonCamera); 
		ScreenEffectComponent->ApplyPostProcessMaterials(ThirdPersonCamera); 
		ScreenEffectComponent->SetScalarParameterValue(TEXT("TeamID"), GenericTeamIdToInteger(MyTeamID));
	}
}

void AOWCharacter::InitAbilityActorInfo()
{
	AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>(); 
	if (!OWPlayerState)
	{
		return;
	}

	UOWAbilitySystemComponent* OWPlayerStateASC = Cast<UOWAbilitySystemComponent>(OWPlayerState->GetAbilitySystemComponent()); 
	if (!OWPlayerStateASC)
	{
		UE_LOG(LogTemp, Error, TEXT("Ability System Component is not initialized properly in AOWCharacter::InitAbilityActorInfo()"));
		return;
	}

	OWPlayerStateASC->InitAbilityActorInfo(OWPlayerState, this); 
	OWPlayerStateASC->AbilityActorInfoSet(); 

	AbilitySystemComponent = OWPlayerStateASC;

	AttributeSet = Cast<UOWAttributeSet>(OWPlayerState->GetAttributeSet()); 
	check(AttributeSet);
	if (AttributeSet)
	{
		AttributeSet->OnDeath.AddUObject(this, &AOWCharacter::HandleDeath); 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute Set is not initialized properly in AOWCharacter::InitAbilityActorInfo()"));
	}

	OnASCRegistered.Broadcast(AbilitySystemComponent); 
	AbilitySystemComponent->RegisterGameplayTagEvent(
		FOWGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AOWCharacter::StunTagChanged); 
}

void AOWCharacter::Reset()
{
	DisableMovementAndCollision(); 

	K2_OnReset(); 

	UninitAndDestroy(); 
}

void AOWCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true); 
	}

	UCapsuleComponent* CharacterCapsule = GetCapsuleComponent(); 
	check(CharacterCapsule);

	CharacterCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterCapsule->SetCollisionResponseToChannels(ECR_Ignore);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->DisableMovement();
	}
}

void AOWCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy(); 
		SetLifeSpan(0.1f); 
	}

	// TODO - Uninitialize Ability System

	SetActorHiddenInGame(true); 
}

void AOWCharacter::HandleDeath(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude)
{
#if WITH_SERVER_CODE 
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Get GameplayTag Singleton Class
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

		// Send Death Gameplay Event 
		FGameplayEventData GameplayEventData; 
		GameplayEventData.EventTag = GameplayTags.Event_Death;
		GameplayEventData.Instigator = DamageInstigator; 
		GameplayEventData.Target = AbilitySystemComponent->GetAvatarActor(); 
		GameplayEventData.OptionalObject = DamageEffectSpec->Def; 
		GameplayEventData.ContextHandle = DamageEffectSpec->GetEffectContext(); 
		GameplayEventData.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags(); 
		GameplayEventData.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags(); 
		GameplayEventData.EventMagnitude = DamageMagnitude; 

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true); 
		AbilitySystemComponent->HandleGameplayEvent(GameplayEventData.EventTag, &GameplayEventData); 

		// Initialize Death Gameplay Message
		FOWVerbMessage OWVerbMessage; 
		OWVerbMessage.Verb = GameplayTags.Gameplay_Message_HeroKilled; 
		OWVerbMessage.Instigator = DamageInstigator; 
		OWVerbMessage.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags(); 
		OWVerbMessage.Target = UOWAbilitySystemLibrary::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor()); 
		OWVerbMessage.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags(); 

		// Broadcast Death Gameplay Message 
		UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld()); 
		GameplayMessageSubsystem.BroadcastMessage(OWVerbMessage.Verb, OWVerbMessage); 
	}
#endif 
}

void AOWCharacter::MulticastLevelUp_Implementation() const
{
	// TODO - Activate Level Up Niagara Component
}
