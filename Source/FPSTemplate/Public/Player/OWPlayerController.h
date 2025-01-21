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
	virtual void BeginPlay() override; 
	virtual void SetupInputComponent() override; 

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> OWContext; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction; 

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UOWInputConfig> InputConfig; 

	UPROPERTY()
	TObjectPtr<UOWAbilitySystemComponent> OWAbilitySystemComponent;

	UOWAbilitySystemComponent* GetAbilitySystemComponent();

	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	void AbilityInputTagPressed(FGameplayTag InputTag); 
	void AbilityInputTagReleased(FGameplayTag InputTag); 
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
