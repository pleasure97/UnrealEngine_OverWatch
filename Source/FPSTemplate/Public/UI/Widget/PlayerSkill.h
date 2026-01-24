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

	/* Widget Animation */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PressedAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ReleasedAnimation;

	/* Widget Design */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlackColor; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor WhiteColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlockedColor; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor ActivatedColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor DeactivatedColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor CooldownColor;

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

	UPROPERTY(BlueprintReadOnly)
	float UpdateInterval = 0.1f;

	void SetCooldownInfo(const FOWAbilityInfo& Info);

	void SetWidgetInfo(const FOWAbilityInfo& WidgetInfo);
	void SetIconInfo(const FOWAbilityInfo& WidgetInfo);
	void SetAbilityStackingInfo(const FOWAbilityInfo& WidgetInfo);
	void SetInputInfo();

	void UpdateActivatedByTag(bool bActivated);
	void UpdateBlockedByTag(bool bBlocked); 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	/* Ability Stacking */
	void UpdateCurrentStacks(const FGameplayAttribute& Attribute, const FOnAttributeChangeData& Data);

	int32 NumCurrentStacks = -1; 

	bool bAbilityStacking = false;

	/* Cooldown */
	UFUNCTION()
	void HandleCooldownTimer(FGameplayTag DurationTag, float TimeRemaining, float Duration);

	UFUNCTION()
	void UpdateCooldownTimer(FGameplayTag DurationTag, float TimeRemaining, float Duration);

	UFUNCTION()
	void EndCooldownTimer(FGameplayTag DurationTag, float TimeRemaining, float Duration);

	bool bCurrentlyActivated = false;
	bool bCurrentlyBlocked = false;
	bool bCurrentlyCooldown = false;
};
