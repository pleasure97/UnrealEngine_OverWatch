// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerSkill.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "OWGameplayTags.h"
#include "AbilitySystem/Abilities/OWGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

void UPlayerSkill::NativeConstruct()
{
	Super::NativePreConstruct(); 

	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(0.f);
	}
	if (TextBlock_Cooltime)
	{
		TextBlock_Cooltime->SetVisibility(ESlateVisibility::Collapsed); 
	}
	if (Overlay_NumCurrentStacks)
	{
		Overlay_NumCurrentStacks->SetVisibility(ESlateVisibility::Collapsed); 
	}
	if (Image_InputKey)
	{
		Image_InputKey->SetVisibility(ESlateVisibility::Collapsed); 
	}
	if (Image_Deactivate)
	{
		Image_Deactivate->SetColorAndOpacity(WhiteColor);
	}
}

void UPlayerSkill::NativeDestruct()
{
	if (IsValid(WaitCooldownChangeTask))
	{
		WaitCooldownChangeTask->OnDurationBegin.RemoveAll(this);
		WaitCooldownChangeTask->OnDurationTimeUpdated.RemoveAll(this);
		WaitCooldownChangeTask->OnDurationEnd.RemoveAll(this);
	}

	Super::NativeDestruct(); 
}

void UPlayerSkill::SetWidgetInfo(const FOWAbilityInfo& WidgetInfo)
{
	// Check if Skill Input Info Map is Set 
	if (SkillInputInfoMap.IsEmpty())
	{
		return;
	}

	// Assign Ability and Input GameplayTag
	AbilityTag = WidgetInfo.AbilityTag;
	InputTag = WidgetInfo.InputTag;
	
	SetIconInfo(WidgetInfo);

	SetInputInfo();

	SetAbilityStackingInfo(WidgetInfo);
}

void UPlayerSkill::SetIconInfo(const FOWAbilityInfo& WidgetInfo)
{
	// Initialize Skill Icon Information 
	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(WidgetInfo.Icon.Get()));
	SlateBrush.TintColor = FSlateColor(BlackColor);
	if (Image_SkillIcon)
	{
		Image_SkillIcon->SetBrush(SlateBrush);
	}
}

void UPlayerSkill::SetAbilityStackingInfo(const FOWAbilityInfo& WidgetInfo)
{
	// Ability Stacking 
	// Check if Widget Info Ability is Not nullptr and Child of OWGameplayAbility 
	if (WidgetInfo.Ability && WidgetInfo.Ability->IsChildOf(UOWGameplayAbility::StaticClass()))
	{
		// Get Class Default Object from Widget Info Ability 
		UOWGameplayAbility* OWGameplayAbility = WidgetInfo.Ability->GetDefaultObject<UOWGameplayAbility>();
		// Get Current Stack Attribute from CDO 
		const FGameplayAttribute StackAttribute = OWGameplayAbility->CurrentStackAttribute;

		// Early Return if Ability Stacking Slot of Custom Gameplay Ability is Not Stacking
		if (OWGameplayAbility->AbilityStackingSlot == EAbilityStackingSlot::None)
		{
			return;
		}

		// Cast Widget Controller to Overlay Widget Controller
		if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
		{
			// Bind Current Stack Attribute Value Changed Delegate of Ability System Component of Overlay Widget Controller
			OverlayWidgetController->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				OWGameplayAbility->CurrentStackAttribute).AddLambda(
					[this, StackAttribute](const FOnAttributeChangeData& Data)
					{
						UpdateCurrentStacks(StackAttribute, Data);
					}
				);
			bAbilityStacking = true;
		}
	}
}

void UPlayerSkill::SetInputInfo()
{
	// Find Input Tag Key from Skill Input Map 
	FSkillInputInfo* SkillInputInfo = SkillInputInfoMap.Find(InputTag);
	if (!SkillInputInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Tag Does Not Match Tag Exact in UPlayerSkill::SetWidgetInfo()"));
		return;
	}

	// Text Block Input Key 
	if (TextBlock_InputKey && !SkillInputInfo->SkillInputText.IsEmpty())
	{
		TextBlock_InputKey->SetText(SkillInputInfo->SkillInputText);
		return;
	}
	// Image Input Key 
	if (Image_InputKey && SkillInputInfo->SkillInputImage)
	{
		// Set Brush of Image Input Key from Texture
		FSlateBrush InputImageBrush;
		InputImageBrush.SetResourceObject(SkillInputInfo->SkillInputImage);
		Image_InputKey->SetBrush(InputImageBrush);
		// Set Visibility of Image Input Key to Visible 
		Image_InputKey->SetVisibility(ESlateVisibility::Visible);
		if (Border_InputKey)
		{
			Border_InputKey->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UPlayerSkill::UpdateActivatedByTag(bool bActivated)
{
	if (bCurrentlyActivated == bActivated)
	{
		return;
	}

	bCurrentlyActivated = bActivated;

	FLinearColor ActivationColor = bActivated ? ActivatedColor : WhiteColor;
	UWidgetAnimation* ActivationAnimation = bActivated ? PressedAnimation : ReleasedAnimation;

	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(ActivationColor);
	}

	if (Image_Deactivate)
	{
		Image_Deactivate->SetColorAndOpacity(ActivationColor);
	}

	if (ActivationAnimation)
	{
		PlayAnimation(ActivationAnimation);
	}
}

void UPlayerSkill::UpdateBlockedByTag(bool bBlocked)
{
	if (bCurrentlyBlocked == bBlocked || bCurrentlyCooldown)
	{
		return;
	}

	bCurrentlyBlocked = bBlocked; 

	FLinearColor BackgroundColor = bBlocked ? BlockedColor : WhiteColor;
	FLinearColor DeactivationColor = bBlocked ? DeactivatedColor : WhiteColor;
	FLinearColor SkillIconColor = bBlocked ? BlockedColor : BlackColor;

	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(BackgroundColor);
	}
	if (Image_Deactivate)
	{
		Image_Deactivate->SetColorAndOpacity(DeactivationColor);
	}
	if (Image_SkillIcon)
	{
		FSlateBrush CurrentBrush = Image_SkillIcon->GetBrush();
		CurrentBrush.TintColor = FSlateColor(SkillIconColor);
		Image_SkillIcon->SetBrush(CurrentBrush);
	}
}

void UPlayerSkill::SetCooldownInfo(const FOWAbilityInfo& Info)
{
	if (InputTag.MatchesTagExact(Info.InputTag))
	{
		CooldownTag = Info.CooldownTag;

		if (WaitCooldownChangeTask)
		{
			WaitCooldownChangeTask->EndTask();
		}

		if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
		{
			FGameplayTagContainer CooldownTagContainer;
			CooldownTagContainer.AddTag(CooldownTag);
			WaitCooldownChangeTask = UWaitCooldownChange::WaitCooldownChange(
				OverlayWidgetController->AbilitySystemComponent, CooldownTagContainer, UpdateInterval);
			WaitCooldownChangeTask->OnDurationBegin.AddDynamic(this, &UPlayerSkill::HandleCooldownTimer); 
			WaitCooldownChangeTask->OnDurationTimeUpdated.AddDynamic(this, &UPlayerSkill::UpdateCooldownTimer);
			WaitCooldownChangeTask->OnDurationEnd.AddDynamic(this, &UPlayerSkill::EndCooldownTimer); 
		}
	}
}

void UPlayerSkill::UpdateCurrentStacks(const FGameplayAttribute& Attribute, const FOnAttributeChangeData& Data)
{
	if (Attribute == UOWAttributeSet::GetFirstSkillCurrentStacksAttribute())
	{
		if (TextBlock_NumCurrentStacks)
		{
			NumCurrentStacks = FMath::TruncToInt(Data.NewValue); 
			TextBlock_NumCurrentStacks->SetText(FText::AsNumber(NumCurrentStacks));
		}
	}

	// Check Overlay Visibility and Set Visiblity to Visible 
	if (Overlay_NumCurrentStacks->GetVisibility() != ESlateVisibility::Visible)
	{
		Overlay_NumCurrentStacks->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerSkill::HandleCooldownTimer(FGameplayTag DurationTag, float TimeRemaining, float Duration)
{
	// Ability Stacking Early Return
	if (bAbilityStacking && NumCurrentStacks > 0)
	{
		return;
	}
	
	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(0.f);
	}

	if (TextBlock_Cooltime && TimeRemaining > 0.f)
	{
		TextBlock_Cooltime->SetText(FText::AsNumber(FMath::RoundToInt(TimeRemaining)));
		TextBlock_Cooltime->SetVisibility(ESlateVisibility::Visible);
	}

	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(CooldownColor);
	}

	if (Image_Deactivate)
	{
		Image_Deactivate->SetColorAndOpacity(CooldownColor);
	}

	if (PressedAnimation)
	{
		PlayAnimation(PressedAnimation);
	}

	bCurrentlyCooldown = true;
}

void UPlayerSkill::UpdateCooldownTimer(FGameplayTag DurationTag, float TimeRemaining, float Duration)
{
	// Ability Stacking Early Return
	if (bAbilityStacking && NumCurrentStacks > 0)
	{
		return;
	}

	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(1.f - TimeRemaining / Duration);
	}

	if (TextBlock_Cooltime && TimeRemaining > 0.f)
	{
		TextBlock_Cooltime->SetText(FText::AsNumber(FMath::FloorToInt(TimeRemaining)));
	}
}

void UPlayerSkill::EndCooldownTimer(FGameplayTag DurationTag, float TimeRemaining, float Duration)
{
	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(0.f);
	}
	if (TextBlock_Cooltime)
	{
		TextBlock_Cooltime->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(WhiteColor);
	}

	if (Image_Deactivate)
	{
		Image_Deactivate->SetColorAndOpacity(WhiteColor);
	}

	if (ReleasedAnimation)
	{
		PlayAnimation(ReleasedAnimation);
	}

	bCurrentlyCooldown = false;
}
