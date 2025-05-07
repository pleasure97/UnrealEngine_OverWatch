// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "Interfaces/LevelUpInterface.h"
#include "OWCharacter.generated.h"

class USpringArmComponent; 
class UCameraComponent;
class UCameraTransitionComponent; 
class UInputComponent; 
class UScreenEffectComponent; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWCharacter : public AOWCharacterBase, public ILevelUpInterface 
{
	GENERATED_BODY()
	
public:
	AOWCharacter(); 

	virtual void BeginPlay() override; 

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override; 

	/* LevelUp Interface */
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override; 
	virtual int32 GetXP_Implementation() const override; 
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override; 
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override; 
	virtual void AddToXP_Implementation(int32 InXP) override; 
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override; 
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override; 
	virtual int32 GetAttributePoints_Implementation() const override; 
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetSpellPoints_Implementation() const override; 
	virtual void LevelUp_Implementation() override; 
	/* End LevelUp Interface*/

	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override; 
	virtual void Die(const FVector& DeathImpulse) override; 
	virtual UAnimInstance* GetFirstPersonMeshAnimInstance_Implementation() const override;
	virtual void TransitionCamera_Implementation(bool bSmoothTransition) override;
	/* End Combat Interface */

	UFUNCTION()
	void OnTeamChanged(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	void InitAbilityActorInfo(); 

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
	/* Camera End */

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraTransitionComponent> CameraTransitionComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScreenEffectComponent> ScreenEffectComponent;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp() const; 

	FTimerHandle DeathTimer; 

	float DeathTime = 9.f; 
};
