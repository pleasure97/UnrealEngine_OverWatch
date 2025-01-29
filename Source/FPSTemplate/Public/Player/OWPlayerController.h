// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "GameplayTagContainer.h"
#include "OWPlayerController.generated.h"

class UInputMappingContext;
class UInputAction; 
class UOWInputConfig; 
class UOWAbilitySystemComponent; 
struct FInputActionValue; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AOWPlayerController : public ADSPlayerController
{
	GENERATED_BODY()
	
public:
	AOWPlayerController(); 

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override; 
	virtual void SetupInputComponent() override; 

	bool bPlayerAlive = false; 

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> OWContext; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UOWInputConfig> InputConfig; 

	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> OWAbilitySystemComponent;

	UOWAbilitySystemComponent* GetAbilitySystemComponent();

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Crouch();
	void Input_Jump();

	void AbilityInputTagPressed(FGameplayTag InputTag); 
	void AbilityInputTagReleased(FGameplayTag InputTag); 
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
