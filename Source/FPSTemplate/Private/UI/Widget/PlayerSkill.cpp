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

	ProgressBar_Cooltime->SetPercent(0.f); 
	TextBlock_Cooltime->SetRenderOpacity(0.f); 
}

void UPlayerSkill::NativeDestruct()
{
	Super::NativeDestruct(); 

	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);

	OverlayWidgetController->AbilityInfoDelegate.RemoveDynamic(this, &UPlayerSkill::ReceiveAbilityInfo);
}


void UPlayerSkill::ReceiveAbilityInfo(const FOWAbilityInfo& Info)
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

void UPlayerSkill::SetWidgetInfo(const FOWAbilityInfo& WidgetInfo)
{
	InputTag = WidgetInfo.InputTag; 
	FSlateBrush SlateBrush; 
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(WidgetInfo.Icon.Get())); 
	SlateBrush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)); 
	SlateBrush.SetImageSize(FVector2D(76.8f, 76.8f)); 
	Image_SkillIcon->SetBrush(SlateBrush);

	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_Skill_1))
	{
		TextBlock_InputTag->SetText(FText::FromString("LSHIFT"));
	}

	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_Skill_2))
	{
		TextBlock_InputTag->SetText(FText::FromString("E"));
	}

	if (InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_Skill_3))
	{
		TextBlock_InputTag->SetText(FText::FromString("F"));
	}

	BindToAbilityInfoDelegate(); 
}

void UPlayerSkill::BindToAbilityInfoDelegate()
{
	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UPlayerSkill::ReceiveAbilityInfo); 
	}
}

void UPlayerSkill::HandleCooldownTimer(float TimeRemaining)
{
	ProgressBar_Cooltime->SetPercent(FMath::FInterpConstantTo(1.f, 0.f, GetWorld()->GetDeltaSeconds(), TimeRemaining)); 
	TextBlock_Cooltime->SetRenderOpacity(1.f);

	RemainedTime = TimeRemaining; 

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UPlayerSkill::UpdateCooldownTimer, TimerFrequency, true); 
}

void UPlayerSkill::UpdateCooldownTimer()
{
	RemainedTime -= TimerFrequency; 

	if (RemainedTime <= 0.f && CooldownTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
	TextBlock_Cooltime->SetText(FText::AsNumber(FMath::FloorToInt(RemainedTime)));
}
