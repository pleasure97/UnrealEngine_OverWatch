// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "OWCharacter.generated.h"

class USpringArmComponent; 
class UCameraComponent; 
class UInputComponent; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWCharacter : public AOWCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AOWCharacter(); 

	virtual void PossessedBy(AController* NewController) override; 
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override; 

private:
	void InitAbilityActorInfo(); 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh; 
};
