// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerSkill.generated.h"

class UImage; 
class UProgressBar; 
class UTextBlock; 
struct FOWAbilityInfo; 
class UWaitCooldownChange; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerSkill : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Background; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillIcon; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Cooltime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Cooltime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_InputKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InputKey;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CooldownTag; 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWaitCooldownChange> WaitCooldownChangeTask; 


	UFUNCTION()
	void ReceiveAbilityInfo(const FOWAbilityInfo& Info); 

	void SetWidgetInfo(const FOWAbilityInfo& WidgetInfo); 

	void SetInputTag(const FGameplayTag& InInputTag); 

	void BindToAbilityInfoDelegate(); 

	UFUNCTION()
	void HandleCooldownTimer(float TimeRemaining); 

	UFUNCTION()
	void UpdateCooldownTimer(); 

	float RemainedTime = 0.f; 

	float ElapsedTime = 0.f; 

	float TimerFrequency = 1.f; 

protected:
	virtual void NativePreConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	FTimerHandle CooldownTimerHandle; 
};
