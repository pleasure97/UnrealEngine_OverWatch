// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerSkill.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "TimerManager.h"
#include "OWGameplayTags.h"

void UPlayerSkill::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(0.f);
	}
	if (TextBlock_Cooltime)
	{
		TextBlock_Cooltime->SetRenderOpacity(0.f); 
	}
}

void UPlayerSkill::SetWidgetInfo(const FOWAbilityInfo& WidgetInfo)
{
	AbilityTag = WidgetInfo.AbilityTag; 
	InputTag = WidgetInfo.InputTag; 
	FSlateBrush SlateBrush; 
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(WidgetInfo.Icon.Get())); 
	SlateBrush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)); 
	SlateBrush.SetImageSize(FVector2D(76.8f, 76.8f)); 
	Image_SkillIcon->SetBrush(SlateBrush);

	/* TODO - Change to Map */
	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_Skill_1))
	{
		TextBlock_InputKey->SetText(FText::FromString("LSHIFT"));
	}

	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_Skill_2))
	{
		TextBlock_InputKey->SetText(FText::FromString("E"));
	}

	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_Skill_3))
	{
		TextBlock_InputKey->SetText(FText::FromString("F"));
	}

	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_LMB))
	{
		TextBlock_InputKey->SetText(FText::FromString(""));
	}
}

void UPlayerSkill::UpdateBlockedByTag(bool bBlocked)
{
	if (bCurrentlyBlocked == bBlocked) return;

	bCurrentlyBlocked = bBlocked; 

	if (bBlocked)
	{
		if (Image_Background)
		{
			Image_Background->SetColorAndOpacity(FLinearColor(0.491021f, 0.026241f, 0.076185f, 1.f));
		}
		if (Image_Deactivate)
		{
			Image_Deactivate->SetColorAndOpacity(FLinearColor(0.168269f, 0.025187f, 0.035601f, 1.f));
		}
		if (Image_SkillIcon)
		{
			FSlateBrush CurrentBrush = Image_SkillIcon->GetBrush();
			CurrentBrush.TintColor = FSlateColor(FLinearColor(0.491f, 0.026f, 0.076f, 1.f));
			Image_SkillIcon->SetBrush(CurrentBrush); 
		}
	}
	else
	{
		if (Image_Background)
		{
			Image_Background->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		if (Image_Deactivate)
		{
			Image_Deactivate->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.f));
		}
		if (Image_SkillIcon)
		{
			FSlateBrush CurrentBrush = Image_SkillIcon->GetBrush();
			CurrentBrush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f));
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
		}
	}
}

void UPlayerSkill::HandleCooldownTimer(float TimeRemaining)
{
	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(1.f);
	}
	if (TextBlock_Cooltime)
	{
		TextBlock_Cooltime->SetRenderOpacity(1.f); 
	}

	CooldownDuration = CurrentRemainedTime = TimeRemaining; 

	TextBlock_Cooltime->SetText(FText::AsNumber(FMath::FloorToInt(CurrentRemainedTime)));

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UPlayerSkill::UpdateCooldownTimer, GetWorld()->GetDeltaSeconds(), true);
}

void UPlayerSkill::UpdateCooldownTimer()
{
	if (CurrentRemainedTime <= 1.f && CooldownTimerHandle.IsValid())
	{
		TextBlock_Cooltime->SetRenderOpacity(0.f); 
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		return; 
	}

	CurrentRemainedTime -= GetWorld()->GetDeltaSeconds(); 

	if (ProgressBar_Cooltime)
	{
		ProgressBar_Cooltime->SetPercent(CurrentRemainedTime / CooldownDuration);
	}

	if (TextBlock_Cooltime)
	{
		if (FMath::FloorToInt(CurrentRemainedTime + GetWorld()->GetDeltaSeconds()) != FMath::FloorToInt(CurrentRemainedTime))
		{
			TextBlock_Cooltime->SetText(FText::AsNumber(FMath::FloorToInt(CurrentRemainedTime)));
		}
	}
}
