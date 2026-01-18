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
#include "Team/OWTeamSubsystem.h"

AOWCharacter::AOWCharacter()
{
	PrimaryActorTick.bCanEverTick = true; 

	// First Person Spring Arm 
	FirstPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>("FirstPersonSpringArm");
	FirstPersonSpringArm->SetupAttachment(GetRootComponent());
	FirstPersonSpringArm->TargetArmLength = 0.f;
	FirstPersonSpringArm->bEnableCameraLag = false;
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
	// Ensure Replication of Camera Transition Component 
	CameraTransitionComponent->SetIsReplicated(true);

	// Screen Effect Component
	ScreenEffectComponent = CreateDefaultSubobject<UScreenEffectComponent>("ScreenEffectComponent");

	// Health Plate Component
	HealthPlateComponent = CreateDefaultSubobject<UWidgetComponent>("HealthPlateComponent");
	HealthPlateComponent->SetupAttachment(GetRootComponent());
	// HealthPlateComponent->SetVisibility(false);

	// Show ThirdPersonMesh visible to other clients, invisible to the user 
	GetMesh()->bOnlyOwnerSee = false; 
	GetMesh()->bOwnerNoSee = true; 
	GetMesh()->bReceivesDecals = false; 
}

void AOWCharacter::BeginPlay()
{
	Super::BeginPlay(); 

}

void AOWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 

	if (ITeamInterface* ControllerWithTeamInterface = Cast<ITeamInterface>(NewController))
	{
		MyTeamID = ControllerWithTeamInterface->GetGenericTeamId();
		if (!ControllerWithTeamInterface->GetTeamChangedDelegate().IsBound())
		{
			ControllerWithTeamInterface->GetTeamChangedDelegate().AddUniqueDynamic(this, &AOWCharacter::OnTeamChanged);
		}
	}

	if (AOWPlayerState* OWPlayerState = GetPlayerState<AOWPlayerState>())
	{
		OWPlayerState->OnRep_HeroName();
	}

	if (IsValid(GetAbilitySystemComponent()) && HasAuthority())
	{
		if (!bPossessed)
		{
			InitAbilityActorInfo();

			AddHeroAbilities();

			InitializeDefaultAttributes();

			bPossessed = true;
		}
	}

	InitializeHealthPlate();
}

void AOWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsValid(GetAbilitySystemComponent()))
	{
		InitAbilityActorInfo();
	}

	if (IsLocallyControlled())
	{
		if (ScreenEffectComponent)
		{
			ScreenEffectComponent->ApplyPostProcessMaterials(FirstPersonCamera);
			ScreenEffectComponent->ApplyPostProcessMaterials(ThirdPersonCamera);
			ScreenEffectComponent->SetScalarParameterValue(TEXT("TeamID"), GenericTeamIdToInteger(MyTeamID));
		}

		InitializeOverlay();

		if (FirstPersonSpringArm)
		{
			FTimerHandle CameraLagTimerHandle;
			GetWorldTimerManager().SetTimer(CameraLagTimerHandle, this, &AOWCharacter::EnableCameraLag, 1.f, false);
		}
	}

	InitializeHealthPlate();
}

void AOWCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set First Person Camera, Third Person Camera, and First Person Mesh of Camera Transition Component 
	// Since they are Not Set Properly in Constructor
	if (CameraTransitionComponent)
	{
		CameraTransitionComponent->SetFirstPersonCamera(FirstPersonCamera);
		CameraTransitionComponent->SetThirdPersonCamera(ThirdPersonCamera);
		CameraTransitionComponent->SetFirstPersonMesh(FirstPersonMesh);
	}
}

UAbilitySystemComponent* AOWCharacter::GetAbilitySystemComponent() const
{
	AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetPlayerState());
	if (IsValid(OWPlayerState))
	{
		return OWPlayerState->GetAbilitySystemComponent();
	}
	else
	{
		return nullptr;
	}
}

UOWAttributeSet* AOWCharacter::GetAttributeSet() const
{
	AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetPlayerState());
	if (IsValid(OWPlayerState))
	{
		return Cast<UOWAttributeSet>(OWPlayerState->GetAttributeSet());
	}
	return nullptr;
}

void AOWCharacter::InitializeDefaultAttributes() const
{
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetPlayerState()))
	{
		UOWAbilitySystemComponent* ASC = Cast<UOWAbilitySystemComponent>(GetAbilitySystemComponent()); 

		if ((HeroName != EHeroName::None) && IsValid(ASC))
		{
			UOWAbilitySystemLibrary::InitializeDefaultAttributes(this, HeroName, ASC);
		}
	}
}

void AOWCharacter::ResetAttributes() const
{
	if (AOWPlayerState* OWPlayerState = Cast<AOWPlayerState>(GetPlayerState()))
	{
		UOWAbilitySystemComponent* ASC = Cast<UOWAbilitySystemComponent>(GetAbilitySystemComponent());

		if ((HeroName != EHeroName::None) && IsValid(ASC))
		{
			UOWAbilitySystemLibrary::ResetAttributes(this, HeroName, ASC);
		}
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
			if (IsValid(GetAbilitySystemComponent()) && IsValid(GetAttributeSet()))
			{
				// Get HUD from Custom Player Controller and Pass Related Values 
				if (AOWHUD* OWHUD = Cast<AOWHUD>(OWPlayerController->GetHUD()))
				{
					OWHUD->InitOverlay(OWPlayerController, OWPlayerState, GetAbilitySystemComponent(), GetAttributeSet());
				}
			}
		}
	}
}

void AOWCharacter::InitializeHealthPlate()
{
	// Get My Player State and Check if it's Valid 
	AOWPlayerState* MyPlayerState = GetPlayerState<AOWPlayerState>(); 
	if (!IsValid(MyPlayerState))
	{
		return;
	}

	// Check if Health Plate Component is Valid 
	if (IsValid(HealthPlateComponent))
	{
		if (HealthPlateComponent->GetWidget() == nullptr)
		{
			HealthPlateComponent->InitWidget();
		}

		// Get Health Plate from Widget Component
		if (UHealthPlate* HealthPlate = Cast<UHealthPlate>(HealthPlateComponent->GetUserWidgetObject()))
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

void AOWCharacter::TransitionCamera_Implementation(bool bFirstPersonView, bool bSmoothTransition)
{
	if (CameraTransitionComponent)
	{
		CameraTransitionComponent->Server_SetViewMode(bFirstPersonView); 
	}
}

UAnimInstance* AOWCharacter::GetFirstPersonMeshAnimInstance_Implementation() const
{
	return FirstPersonMesh->GetAnimInstance(); 
}

UCameraComponent* AOWCharacter::GetFirstPersonCamera() const
{
	return FirstPersonCamera;
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

void AOWCharacter::EnableCameraLag()
{
	if (FirstPersonSpringArm)
	{
		FirstPersonSpringArm->bEnableCameraLag = true;
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

	UOWAttributeSet* AttributeSet = GetAttributeSet();
	if (AttributeSet)
	{
		AttributeSet->OnDeath.AddUObject(this, &AOWCharacter::HandleDeath); 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute Set is not initialized properly in AOWCharacter::InitAbilityActorInfo()"));
	}

	OnASCRegistered.Broadcast(OWPlayerStateASC);
	OWPlayerStateASC->RegisterGameplayTagEvent(
		FOWGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AOWCharacter::StunTagChanged); 
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
	CollisionResponseContainer = CharacterCapsule->GetCollisionResponseToChannels(); 
	CharacterCapsule->SetCollisionResponseToChannels(ECR_Ignore);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->DisableMovement();
	}
}

void AOWCharacter::Revive()
{
	EnableMovementAndCollision();

	bDead = false; 
}

bool AOWCharacter::IsPossessed() const
{
	return bPossessed;
}

EHeroName AOWCharacter::GetHeroName() const
{
	return HeroName;
}

FTransform AOWCharacter::GetOriginalTransform() const
{
	return OriginalTransform;
}

void AOWCharacter::SetOriginalTransform(FTransform InOriginalTransform)
{
	OriginalTransform = InOriginalTransform;
}

void AOWCharacter::EnableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(false);
	}

	UCapsuleComponent* CharacterCapsule = GetCapsuleComponent();
	check(CharacterCapsule);

	CharacterCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CharacterCapsule->SetCollisionResponseToChannels(CollisionResponseContainer);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AOWCharacter::UpdateHealthPlateVisibility()
{
	// Check if Health Plate Component is Valid 
	if (!IsValid(HealthPlateComponent))
	{
		return;
	}

	// Get Local Player Controller and Check if it is Valid 
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(LocalPlayerController))
	{
		return;
	}

	// Get Team Subsystem and Compare Local Player Controller and Current Character 
	EOWTeamComparison TeamComparison = EOWTeamComparison::InvalidArgument;
	if (UOWTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UOWTeamSubsystem>())
	{
		TeamComparison = TeamSubsystem->CompareTeams(this, LocalPlayerController);
	}

	switch (TeamComparison)
	{
	case EOWTeamComparison::OnSameTeam:
		HealthPlateComponent->SetVisibility(true);
		UE_LOG(LogTemp, Log, TEXT("OnSameTeam in AOWCharacter::UpdateHealthPlateVisibility()"));
		return;
	case EOWTeamComparison::DifferentTeams:
		bool bVisibleNow = WasRecentlyRendered(0.2f); 
		HealthPlateComponent->SetVisibility(bVisibleNow);
		UE_LOG(LogTemp, Log, TEXT("DifferentTeams in AOWCharacter::UpdateHealthPlateVisibility()"));
		return;
	}
}

void AOWCharacter::HandleDeath(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude)
{
#if WITH_SERVER_CODE 
	if (bDead)
	{
		return;
	}

	bDead = true;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && DamageEffectSpec)
	{
		// Get GameplayTag Singleton Class
		const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

		// Send Death Gameplay Event 
		FGameplayEventData GameplayEventData; 
		GameplayEventData.EventTag = GameplayTags.Event_Death;
		GameplayEventData.Instigator = DamageInstigator; 
		GameplayEventData.Target = ASC->GetAvatarActor();
		GameplayEventData.OptionalObject = DamageEffectSpec->Def; 
		GameplayEventData.ContextHandle = DamageEffectSpec->GetEffectContext(); 
		GameplayEventData.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags(); 
		GameplayEventData.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags(); 
		GameplayEventData.EventMagnitude = DamageMagnitude; 

		FScopedPredictionWindow NewScopedWindow(ASC, true);
		ASC->HandleGameplayEvent(GameplayEventData.EventTag, &GameplayEventData);

		// Initialize Death Gameplay Message
		FOWVerbMessage OWVerbMessage; 
		OWVerbMessage.Verb = GameplayTags.Gameplay_Message_HeroKilled; 
		OWVerbMessage.Instigator = DamageInstigator; 
		OWVerbMessage.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags(); 
		OWVerbMessage.Target = UOWAbilitySystemLibrary::GetPlayerStateFromObject(ASC->GetAvatarActor());
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
