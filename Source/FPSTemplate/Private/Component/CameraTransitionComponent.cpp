// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CameraTransitionComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

UCameraTransitionComponent::UCameraTransitionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Create subobjects
    FirstPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersonSpringArm")); 
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh")); 

    ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));

    FPtoTPCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FPtoTPCameraTimeline;"));
    TPtoFPCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TPtoFPCameraTimeline"));

    bFirstPersonView = true;
}

void UCameraTransitionComponent::BeginPlay()
{
    Super::BeginPlay(); 

    // Cache Owning Character
    OwningCharacter = Cast<ACharacter>(GetOwner()); 

    // Cache initial transforms
    FirstPersonCameraTransform = FirstPersonCamera->GetComponentTransform();
    ThirdPersonCameraTransform = ThirdPersonCamera->GetComponentTransform();

    // Set Camera Timeline Play Rate 
    if (FPtoTPCameraTimeline)
    {
        FPtoTPCameraTimeline->SetPlayRate(CameraTransitionSpeed);
    }
    if (TPtoFPCameraTimeline)
    {
        TPtoFPCameraTimeline->SetPlayRate(CameraTransitionSpeed);
    }
}

void UCameraTransitionComponent::SetFirstPersonCameraTransform(const FTransform InTransform)
{
    FirstPersonCameraTransform = InTransform; 
}

void UCameraTransitionComponent::SetThirdPersonCameraTransform(const FTransform InTransform)
{
    ThirdPersonCameraTransform = InTransform; 
}

void UCameraTransitionComponent::ActiveFirstPersonCamera()
{
    if (!OwningCharacter) { return; }

    OwningCharacter->bUseControllerRotationYaw = true;
    ThirdPersonCamera->SetActive(false);
    FirstPersonCamera->SetActive(true);
    bFirstPersonView = true;
}

void UCameraTransitionComponent::ActiveThirdPersonCamera()
{
    if (!OwningCharacter) { return; }

    OwningCharacter->bUseControllerRotationYaw = false;
    FirstPersonCamera->SetActive(false);
    ThirdPersonCamera->SetActive(true);
    bFirstPersonView = false;
}

void UCameraTransitionComponent::UpdateFPtoTPCamera(float Output)
{
    FTransform NewTransform = UKismetMathLibrary::TLerp(
        FirstPersonCamera->GetComponentTransform(), 
        ThirdPersonCamera->GetComponentTransform(), 
        Output);
    FirstPersonCamera->SetWorldTransform(NewTransform);
}

void UCameraTransitionComponent::FinishFPtoTPCamera()
{
    ActiveThirdPersonCamera();
    FirstPersonCamera->SetRelativeTransform(FirstPersonCameraTransform);
    bCameraTransitioning = false;
}

void UCameraTransitionComponent::UpdateTPtoFPCamera(float Output)
{
    if (!OwningCharacter) { return; }
    UCapsuleComponent* CapsuleComponent = OwningCharacter->GetCapsuleComponent(); 

    FVector NewVector = UKismetMathLibrary::VLerp(
        ThirdPersonCamera->GetComponentLocation(),
        FirstPersonCamera->GetComponentLocation(), 
        Output);
    ThirdPersonCamera->SetWorldLocation(NewVector);

    FRotator NewRotator = UKismetMathLibrary::RLerp(
        CapsuleComponent->GetComponentRotation(),
        ThirdPersonCamera->GetComponentRotation(), 
        Output, false);
    CapsuleComponent->SetWorldRotation(NewRotator);
}

void UCameraTransitionComponent::FinishTPtoFPCamera()
{
    ActiveFirstPersonCamera();
    ThirdPersonCamera->SetRelativeTransform(ThirdPersonCameraTransform);
    bCameraTransitioning = false;
}

void UCameraTransitionComponent::TransitionCamera(bool bSmoothTransition)
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

            if (OwningCharacter->IsLocallyControlled())
            {
                OwningCharacter->GetMesh()->SetVisibility(true, true);
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

            if (OwningCharacter->IsLocallyControlled())
            {
                OwningCharacter->GetMesh()->SetVisibility(false, true);
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