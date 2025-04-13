// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerSkill.generated.h"

class UImage; 
class UProgressBar; 
class UTextBlock; 
class UBorder;
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
	TObjectPtr<UImage> Image_Deactivate; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillIcon; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Cooltime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Cooltime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_InputKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InputKey;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityTag; 

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CooldownTag; 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWaitCooldownChange> WaitCooldownChangeTask; 

	void SetWidgetInfo(const FOWAbilityInfo& WidgetInfo); 

	void UpdateBlockedByTag(bool bBlocked); 

	void SetCooldownInfo(const FOWAbilityInfo& Info); 

	UFUNCTION()
	void HandleCooldownTimer(float TimeRemaining); 

	UFUNCTION()
	void UpdateCooldownTimer(); 

	float CurrentRemainedTime = 0.f; 

	float CooldownDuration = 0.f; 

	float TimerFrequency = 1.f; 

protected:
	virtual void NativePreConstruct() override; 

private:
	FTimerHandle CooldownTimerHandle; 

	bool bCurrentlyBlocked = false; 
};
