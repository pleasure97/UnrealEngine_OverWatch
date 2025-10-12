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

void UPlayerSkill::NativePreConstruct()
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
}

void UPlayerSkill::NativeDestruct()
{
	if (IsValid(WaitCooldownChangeTask))
	{
		WaitCooldownChangeTask->CooldownStart.RemoveAll(this); 
		WaitCooldownChangeTask->CooldownEnd.RemoveAll(this); 
	}

	Super::NativeDestruct(); 
}

void UPlayerSkill::SetWidgetInfo(const FOWAbilityInfo& WidgetInfo)
{
	// Assign Ability and Input GameplayTag
	AbilityTag = WidgetInfo.AbilityTag;
	InputTag = WidgetInfo.InputTag;
	
	// Initialize Skill Icon Information 
	FSlateBrush SlateBrush; 
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(WidgetInfo.Icon.Get())); 
	SlateBrush.TintColor = FSlateColor(BlackColor);
	if (Image_SkillIcon)
	{
		Image_SkillIcon->SetBrush(SlateBrush);
	}

	// Get GameplayTag Singleton Container
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
	// Check if Skill Input Info Map is SEt 
	if (!SkillInputInfoMap.IsEmpty())
	{
		// Find Input Tag Key from Skill Input Map 
		if (FSkillInputInfo* SkillInputInfo = SkillInputInfoMap.Find(InputTag))
		{
			// Text Block Input Key 
			if (TextBlock_InputKey && !SkillInputInfo->SkillInputText.IsEmpty())
			{
				TextBlock_InputKey->SetText(SkillInputInfo->SkillInputText); 
			}
			// Image Input Key 
			if (Image_InputKey && !SkillInputInfo->SkillInputImage.IsNull())
			{
				// Set Brush of Image Input Key from Texture
				Image_InputKey->SetBrushFromTexture(SkillInputInfo->SkillInputImage, false); 
				// Set Visibility of Image Input Key to Visible 
				Image_InputKey->SetVisibility(ESlateVisibility::Visible); 
				// Set Visiblity of Border Input Key to Collapsed 
				if (Border_InputKey)
				{
					Border_InputKey->SetVisibility(ESlateVisibility::Collapsed); 
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Input Tag Does Not Match Tag Exact in UPlayerSkill::SetWidgetInfo()"));
		}

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
}

void UPlayerSkill::UpdateBlockedByTag(bool bBlocked)
{
	if (bCurrentlyBlocked == bBlocked)
	{
		return;
	}

	bCurrentlyBlocked = bBlocked; 

	if (bBlocked)
	{
		if (Image_Background)
		{
			Image_Background->SetColorAndOpacity(BlockedColor);
		}
		if (Image_Deactivate)
		{
			Image_Deactivate->SetColorAndOpacity(DeactivatedColor);
			Image_Deactivate->SetVisibility(ESlateVisibility::Visible);
		}
		if (Image_SkillIcon)
		{
			FSlateBrush CurrentBrush = Image_SkillIcon->GetBrush();
			CurrentBrush.TintColor = FSlateColor(BlockedColor);
			Image_SkillIcon->SetBrush(CurrentBrush); 
		}
	}
	else
	{
		if (Image_Background)
		{
			Image_Background->SetColorAndOpacity(WhiteColor);
		}
		if (Image_Deactivate)
		{
			Image_Deactivate->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (Image_SkillIcon)
		{
			FSlateBrush CurrentBrush = Image_SkillIcon->GetBrush();
			CurrentBrush.TintColor = FSlateColor(BlackColor);
			Image_SkillIcon->SetBrush(CurrentBrush);
		}
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
			WaitCooldownChangeTask = UWaitCooldownChange::WaitForCooldownChange(OverlayWidgetController->AbilitySystemComponent, CooldownTag);
			WaitCooldownChangeTask->CooldownStart.AddDynamic(this, &UPlayerSkill::HandleCooldownTimer); 
			WaitCooldownChangeTask->CooldownEnd.AddDynamic(this, &UPlayerSkill::EndCooldownTimer); 
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

void UPlayerSkill::HandleCooldownTimer(float TimeRemaining)
{
	// Ability Stacking Early Return
	if (bAbilityStacking && NumCurrentStacks > 0)
	{
		return;
	}
	
	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(1.f);
	}

	CooldownDuration = CurrentRemainedTime = TimeRemaining; 

	if (TextBlock_Cooltime)
	{
		TextBlock_Cooltime->SetText(FText::AsNumber(FMath::RoundToInt(CurrentRemainedTime)));
		TextBlock_Cooltime->SetVisibility(ESlateVisibility::Visible);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			CooldownTimerHandle, 
			this, 
			&UPlayerSkill::UpdateCooldownTimer, 
			World->GetDeltaSeconds(), 
			true);
	}
}

void UPlayerSkill::UpdateCooldownTimer()
{
	if (UWorld* World = GetWorld())
	{
		if (CurrentRemainedTime <= 0.1f && CooldownTimerHandle.IsValid())
		{
			World->GetTimerManager().ClearTimer(CooldownTimerHandle);
			return;
		}

		CurrentRemainedTime -= World->GetDeltaSeconds();

		if (ProgressBar_Cooltime)
		{
			ProgressBar_Cooltime->SetPercent(CurrentRemainedTime / CooldownDuration);
		}

		if (TextBlock_Cooltime)
		{
			if (FMath::FloorToInt(CurrentRemainedTime + World->GetDeltaSeconds()) != FMath::FloorToInt(CurrentRemainedTime))
			{
				TextBlock_Cooltime->SetText(FText::AsNumber(FMath::FloorToInt(CurrentRemainedTime)));
			}
		}
	}
}

void UPlayerSkill::EndCooldownTimer(float TimeRemaining)
{
	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(0.f);
	}
	if (TextBlock_Cooltime)
	{
		TextBlock_Cooltime->SetVisibility(ESlateVisibility::Collapsed);
	}
}
