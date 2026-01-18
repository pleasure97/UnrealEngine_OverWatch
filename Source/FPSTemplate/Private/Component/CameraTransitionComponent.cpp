// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CameraTransitionComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UCameraTransitionComponent::UCameraTransitionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCameraTransitionComponent::BeginPlay()
{
    Super::BeginPlay(); 

    // Cache Owning Character
    OwningCharacter = Cast<ACharacter>(GetOwner()); 

    OnRep_FirstPersonView();
}

void UCameraTransitionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCameraTransitionComponent, bFirstPersonView); 
}

/* Getter & Setter */
bool UCameraTransitionComponent::IsFirstPersonView() const
{
    return bFirstPersonView;
}

void UCameraTransitionComponent::SetFirstPersonCamera(UCameraComponent* InFirstPersonCamera)
{
    FirstPersonCamera = InFirstPersonCamera;
}

void UCameraTransitionComponent::SetThirdPersonCamera(UCameraComponent* InThirdPersonCamera)
{
    ThirdPersonCamera = InThirdPersonCamera;
}


void UCameraTransitionComponent::SetFirstPersonMesh(USkeletalMeshComponent* InFirstPersonMesh)
{
    FirstPersonMesh = InFirstPersonMesh;
}
/* Getter & Setter End */

void UCameraTransitionComponent::OnRep_FirstPersonView()
{
    UpdatePerspective();
}

void UCameraTransitionComponent::UpdatePerspective()
{
    UpdateMeshVisibility();

    bFirstPersonView ? ActiveFirstPersonCamera() : ActiveThirdPersonCamera();
}

void UCameraTransitionComponent::ActiveFirstPersonCamera()
{
    // Check if Owning Character is Valid 
    if (!IsValid(OwningCharacter)) 
    {
        return; 
    }

    // Allow Free Use of Controller Rotation Yaw in First Person Perspective 
    OwningCharacter->bUseControllerRotationYaw = true;
    // Activate First Person Camera, and Deactivate Third Person Camera
    ThirdPersonCamera->SetActive(false);
    FirstPersonCamera->SetActive(true);
}

void UCameraTransitionComponent::ActiveThirdPersonCamera()
{
    // Check if Owning Character is Valid 
    if (!IsValid(OwningCharacter))
    {
        return; 
    }

    // Limit Controller Rotation Yaw to Avoid Losing Sight of Owning Character in Third Person Perspective
    OwningCharacter->bUseControllerRotationYaw = false;
    // Activate Third Person Camera, and Deactivate First Person Camera
    FirstPersonCamera->SetActive(false);
    ThirdPersonCamera->SetActive(true);
}

void UCameraTransitionComponent::UpdateMeshVisibility()
{
    // Check if Owning Character is Valid 
    if (!IsValid(OwningCharacter))
    {
        return;
    }

    // Get Owning Character Mesh
    USkeletalMeshComponent* OwningCharacterMesh = OwningCharacter->GetMesh(); 

    // Check if First Person Mesh and Owning Character Mesh are Valid 
    if (!IsValid(OwningCharacterMesh) || !IsValid(FirstPersonMesh))
    {
        return;
    }

    // Check if Owning Character is Locally Controlled
    if (!OwningCharacter->IsLocallyControlled())
    {
        return;
    }

    if (bFirstPersonView)
    {
        OwningCharacterMesh->SetVisibility(false, true);
        // Show ThirdPersonMesh visible to other clients, invisible to the user 
        OwningCharacterMesh->bOnlyOwnerSee = false;
        OwningCharacterMesh->bOwnerNoSee = true;
        OwningCharacterMesh->bReceivesDecals = false;
        FirstPersonMesh->SetVisibility(true, true);
    }
    else
    {
        OwningCharacterMesh->SetVisibility(true, true);
        // Show ThirdPersonMesh visible to other clients, invisible to the user 
        OwningCharacterMesh->bOnlyOwnerSee = true;
        OwningCharacterMesh->bOwnerNoSee = false;
        OwningCharacterMesh->bReceivesDecals = true;
        FirstPersonMesh->SetVisibility(false, true);
    }

    // Notify Rendering Thread that Rendering Flag State Has Changed  
    OwningCharacterMesh->MarkRenderStateDirty(); 
    FirstPersonMesh->MarkRenderStateDirty();
}

void UCameraTransitionComponent::Server_SetViewMode_Implementation(bool bNewFirstPerson)
{
    bFirstPersonView = bNewFirstPerson;

    UpdatePerspective();
}
