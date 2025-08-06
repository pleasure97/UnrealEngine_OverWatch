// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "TracerBlinkWidget.generated.h"

class UOWGameplayAbility; 
class UAbilitySystemComponent; 
class UProgressBar; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UTracerBlinkWidget : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UOWGameplayAbility> BlinkAbilityClass; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Blink; 
	
protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComponent;

	UFUNCTION()
	void OnBlinkStackChanged(const FGameplayTag CooldownTag, int32 NewCount); 
};
