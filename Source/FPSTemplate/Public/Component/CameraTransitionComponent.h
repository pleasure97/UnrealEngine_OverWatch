// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraTransitionComponent.generated.h"

class ACharacter; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEMPLATE_API UCameraTransitionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraTransitionComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Getter & Setter
	bool IsFirstPersonView() const;

	void SetFirstPersonCamera(UCameraComponent* InFirstPersonCamera); 
	void SetThirdPersonCamera(UCameraComponent* InThirdPersonCamera);
	void SetFirstPersonMesh(USkeletalMeshComponent* InFirstPersonMesh);

	/* Camera Transition */
	UFUNCTION(Server, Reliable)
	void Server_SetViewMode(bool bNewFirstPerson);

	void UpdatePerspective();
protected:
	/* Camera & First Person Mesh */
	UPROPERTY()
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY()
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;
	/* Camera & First Person Mesh End */

	UPROPERTY()
	TObjectPtr<ACharacter> OwningCharacter; 

	UPROPERTY(ReplicatedUsing = OnRep_FirstPersonView)
	bool bFirstPersonView = true;

	UFUNCTION()
	void OnRep_FirstPersonView();

	void ActiveFirstPersonCamera();

	void ActiveThirdPersonCamera();

	void UpdateMeshVisibility();
};
