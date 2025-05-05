// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
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

	// Getter 
	bool IsFirstPersonView() const { return bFirstPersonView; }

	/* Camera Transition */
	void SetFirstPersonCameraTransform(const FTransform InTransform);
	void SetThirdPersonCameraTransform(const FTransform InTransform);

	void ActiveFirstPersonCamera();
	void ActiveThirdPersonCamera();

	void UpdateFPtoTPCamera(float Output);
	void UpdateTPtoFPCamera(float Output);

	void FinishFPtoTPCamera();
	void FinishTPtoFPCamera();

	void TransitionCamera(bool bSmoothTransition);
	/* Camera Transition End*/

	/* Camera Timeline*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Transition")
	TObjectPtr<UCurveFloat> SlideAmount;

	UPROPERTY(VisibleAnywhere, Category = "Camera Transition")
	TObjectPtr<UTimelineComponent> FPtoTPCameraTimeline;

	UPROPERTY(VisibleAnywhere, Category = "Camera Transition")
	TObjectPtr<UTimelineComponent> TPtoFPCameraTimeline;
	/* End Camera Timeline */

	/* Camera & Spring Arm */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> FirstPersonSpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> ThirdPersonSpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> ThirdPersonCamera;
	/* Camera & Spring Arm End */

private:
	TObjectPtr<ACharacter> OwningCharacter; 

	UPROPERTY()
	FTransform FirstPersonCameraTransform;

	UPROPERTY()
	FTransform ThirdPersonCameraTransform;

	bool bCameraTransitioning = false;

	bool bFirstPersonView = true;

	UPROPERTY()
	float CameraTransitionSpeed = 0.025f;
};
