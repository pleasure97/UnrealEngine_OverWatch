// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OWCharacterBase.h"
#include "Interface/LevelUpInterface.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "OWCharacter.generated.h"

class USpringArmComponent; 
class UCameraComponent;
class UCameraTransitionComponent; 
class UScreenEffectComponent; 
class UWidgetComponent;
struct FGameplayEffectSpec; 
class UOWAttributeSet;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWCharacter : public AOWCharacterBase, public ILevelUpInterface 
{
	GENERATED_BODY()
	
public:
	AOWCharacter(); 

	virtual void Restart() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

	/* Ability System Component & Attribute Set */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UOWAttributeSet* GetAttributeSet() const override;

	UFUNCTION(BlueprintCallable)
	virtual void ResetAttributes() const;

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
	virtual void TransitionCamera_Implementation(bool bFirstPersonView, bool bSmoothTransition) override;

	virtual UAnimInstance* GetFirstPersonMeshAnimInstance_Implementation() const override;
	/* End Combat Interface */

	/* Getter for Component */
	UCameraComponent* GetFirstPersonCamera() const; 

	UFUNCTION()
	void OnTeamChanged(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	UFUNCTION(BlueprintCallable)
	void DisableMovementAndCollision();

	UFUNCTION(BlueprintCallable)

	void Revive(); 

	bool IsPossessed() const;

	EHeroName GetHeroName() const;

	FTransform GetOriginalTransform() const;

	void SetOriginalTransform(FTransform InOriginalTransform);

protected:
	virtual void InitializeDefaultAttributes() const; 

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

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraTransitionComponent> CameraTransitionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	EHeroName HeroName = EHeroName::None;

	UPROPERTY()
	FTransform OriginalTransform;

private:
	void InitAbilityActorInfo(); 

	void InitializeOverlay(); 

	void InitializeHealthPlate();

	void HandleDeath(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude);

	void EnableMovementAndCollision();

	void UpdateHealthPlateVisibility();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScreenEffectComponent> ScreenEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HealthPlateComponent;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp() const; 

	bool bPossessed = false; 

	FTimerHandle HealthPlateTimerHandle;

	UPROPERTY()
	FCollisionResponseContainer CollisionResponseContainer;
	void EnableCameraLag();
};
