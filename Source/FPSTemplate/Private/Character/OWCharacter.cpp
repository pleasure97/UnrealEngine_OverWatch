// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/CameraTransitionComponent.h"
#include "Component/ScreenEffectComponent.h"
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
#include "Game/OWGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Message/OWMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"

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

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("ThirdPersonSpringArm"); 
	ThirdPersonSpringArm->SetupAttachment(GetRootComponent());

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera"); 
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCamera->SetRelativeLocation(FVector(0., 10., 53.));
	ThirdPersonCamera->bUsePawnControlRotation = true;

	CameraTransitionComponent = CreateDefaultSubobject<UCameraTransitionComponent>("CameraTransitionComponent");
	CameraTransitionComponent->FirstPersonSpringArm = FirstPersonSpringArm;
	CameraTransitionComponent->FirstPersonCamera = FirstPersonCamera;
	CameraTransitionComponent->ThirdPersonSpringArm = ThirdPersonSpringArm; 
	CameraTransitionComponent->ThirdPersonCamera = ThirdPersonCamera;

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
		FirstPersonMesh->bOnlyOwnerSee = true;
		FirstPersonMesh->bOwnerNoSee = false;
	}
	else
	{
		FirstPersonMesh->bOnlyOwnerSee = true;
		FirstPersonMesh->bOwnerNoSee = false;
	}
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	FirstPersonMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
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

	if (IsLocallyControlled())
	{
		ScreenEffectComponent->ApplyPostProcessMaterials(FirstPersonCamera);
		ScreenEffectComponent->ApplyPostProcessMaterials(ThirdPersonCamera);
		ScreenEffectComponent->SetScalarParameterValue(TEXT("TeamID"), GenericTeamIdToInteger(MyTeamID));
	}

	InitAbilityActorInfo(); 
	AddHeroAbilities(); 

	InitializeDefaultAttributes();

	// TODO - Give Default Abilities, Give Default Effects 

	if (UOWAttributeSet* OWAttributeSet = Cast<UOWAttributeSet>(AttributeSet))
	{
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

void AOWCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse); 

	CameraTransitionComponent->ActiveThirdPersonCamera(); 

	FTimerDelegate DeathTimerDelegate; 
	DeathTimerDelegate.BindLambda(
		[this]()
		{
			AOWGameModeBase* OWGameMode = Cast<AOWGameModeBase>(UGameplayStatics::GetGameMode(this));
			if (OWGameMode)
			{
				OWGameMode->PlayerDied(this);
			}
		}); 
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
}

UAnimInstance* AOWCharacter::GetFirstPersonMeshAnimInstance_Implementation() const
{
	return FirstPersonMesh->GetAnimInstance(); 
}

void AOWCharacter::TransitionCamera_Implementation(bool bSmoothTransition)
{
	CameraTransitionComponent->TransitionCamera(bSmoothTransition); 
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
	check(OWPlayerState); 

	UOWAbilitySystemComponent* OWPlayerStateASC = Cast<UOWAbilitySystemComponent>(OWPlayerState->GetAbilitySystemComponent()); 
	check(OWPlayerStateASC);

	if (OWPlayerStateASC)
	{
		OWPlayerStateASC->InitAbilityActorInfo(OWPlayerState, this); 
		OWPlayerStateASC->AbilityActorInfoSet(); 
		AbilitySystemComponent = OWPlayerStateASC; 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ability System Component is not initialized properly in AOWCharacter::InitAbilityActorInfo()")); 
	}

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

	if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetController()))
	{
		if (AOWHUD* OWHUD = Cast<AOWHUD>(OWPlayerController->GetHUD()))
		{
			OWHUD->InitOverlay(OWPlayerController, OWPlayerState, AbilitySystemComponent, AttributeSet); 
		}
	}
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
