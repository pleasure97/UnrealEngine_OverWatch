// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "Interfaces/LevelUpInterface.h"
#include "OWCharacter.generated.h"

class USpringArmComponent; 
class UCameraComponent; 
class UInputComponent; 
/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWCharacter : public AOWCharacterBase, public ILevelUpInterface 
{
	GENERATED_BODY()
	
public:
	AOWCharacter(); 

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override; 

	/* LevelUp Interface */
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override; 
	virtual int32 GetXP_Implementation() const override; 
	virtual void AddToXP_Implementation(int32 InXP) override; 
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override; 
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override; 
	virtual int32 GetAttributePoints_Implementation() const override; 
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetSpellPoints_Implementation() const override; 
	virtual void LevelUp_Implementation() override; 
	/* End LevelUp Interface*/
private:
	void InitAbilityActorInfo(); 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh; 

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp() const; 
};
