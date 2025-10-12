// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "PlayerSkill.generated.h"

class UImage; 
class UProgressBar; 
class UTextBlock; 
class UBorder;
class UOverlay;
struct FOWAbilityInfo; 
class UWaitCooldownChange; 

USTRUCT(BlueprintType)
struct FSkillInputInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillInputText; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillInputImage; 
};

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UPlayerSkill : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Widget Bindings */
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_InputKey;

	/* Widget Design */
	UPROPERTY(EditDefaultsOnly)
	FLinearColor BlackColor = FLinearColor(0.f, 0.f, 0.f, 1.f); 

	UPROPERTY(EditDefaultsOnly)
	FLinearColor WhiteColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BlockedColor = FLinearColor(0.491021f, 0.026241f, 0.076185f, 1.f); 

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DeactivatedColor = FLinearColor(0.168269f, 0.025187f, 0.035601f, 1.f);

	/* Widget GameplayTags */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityTag; 

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CooldownTag; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FSkillInputInfo> SkillInputInfoMap; 

	/* Ability Stacking */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UOverlay> Overlay_NumCurrentStacks; 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTextBlock> TextBlock_NumCurrentStacks;

	/* Wait Cooldon Change */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWaitCooldownChange> WaitCooldownChangeTask; 

	void SetCooldownInfo(const FOWAbilityInfo& Info);

	void SetWidgetInfo(const FOWAbilityInfo& WidgetInfo); 

	void UpdateBlockedByTag(bool bBlocked); 

protected:
	virtual void NativePreConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	/* Ability Stacking */
	void UpdateCurrentStacks(const FGameplayAttribute& Attribute, const FOnAttributeChangeData& Data);

	int32 NumCurrentStacks = -1; 

	bool bAbilityStacking = false;

	/* Cooldown */
	UFUNCTION()
	void HandleCooldownTimer(float TimeRemaining);

	UFUNCTION()
	void UpdateCooldownTimer();

	UFUNCTION()
	void EndCooldownTimer(float TimeRemaining);

	FTimerHandle CooldownTimerHandle; 

	float CurrentRemainedTime = 0.f;

	float CooldownDuration = 0.f;

	bool bCurrentlyBlocked = false;

};
