// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Timers/TimerWidget.h"
#include "Player/DSPlayerController.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/TextBlock.h"

void UTimerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwningPlayerController = Cast<ADSPlayerController>(GetOwningPlayer());
	if (IsValid(OwningPlayerController))
	{
		OwningPlayerController->OnTimerUpdated.AddDynamic(this, &UTimerWidget::OnTimerUpdated);
		OwningPlayerController->OnTimerStopped.AddDynamic(this, &UTimerWidget::OnTimerStopped); 
	}

	if (bHiddenWhenInactive)
	{
		TextBlock_Time->SetRenderOpacity(0.f); 
	}
}

void UTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 

	if (bActive)
	{
		switch (TimerDirection)
		{
		case ECountTimerDirection::Countup:
		{
			UpdateCountTime(InternalCountTime + InDeltaTime); 
			break;
		}
		case ECountTimerDirection::Countdown:
		{
			UpdateCountTime(InternalCountTime - InDeltaTime);
			break;
		}
		}
	}
}

void UTimerWidget::OnTimerUpdated(float CountTime, ECountTimerDirection Direction, ECountTimerType Type)
{
	if (Type != TimerType) return; 

	if (!bActive)
	{
		TimerStarted(CountTime);	
	}
	UpdateCountTime(CountTime); 
	K2_OnTimerUpdated(CountTime, Direction, TimerType);
}

void UTimerWidget::OnTimerStopped(float CountdownTimeLeft, ECountTimerDirection Direction, ECountTimerType Type)
{
	if (Type != TimerType) return; 

	TimerStopped(); 

	K2_OnTimerStopped(0.f, Direction, TimerType);
}

FString UTimerWidget::FormatTimeAsString(float TimeSeconds) const
{
	TimeSeconds = bCanBeNegative ? TimeSeconds : FMath::Abs(TimeSeconds); 
	FString DisplayTimeString; 
	if (bShowCentiSeconds)
	{
		DisplayTimeString = UKismetStringLibrary::TimeSecondsToString(TimeSeconds);
	}
	else
	{
		const TCHAR* NegativeModifier = TimeSeconds < 0.f ? TEXT("-") : TEXT("");
		TimeSeconds = FMath::Abs(TimeSeconds);

		const int32 NumMinutes = FMath::FloorToInt(TimeSeconds / 60.f);
		const int32 NumSeconds = FMath::FloorToInt(TimeSeconds - (NumMinutes * 60.f));
		DisplayTimeString = FString::Printf(TEXT("%s%02d:%02d"), NegativeModifier, NumMinutes, NumSeconds);
	}
	return DisplayTimeString; 
}

void UTimerWidget::TimerStarted(float InitialTime)
{
	bActive = true; 
	if (TextBlock_Time)
	{
		TextBlock_Time->SetRenderOpacity(1.f);
	}
	K2_OnTimerStarted(InitialTime, TimerDirection, TimerType); 
}

void UTimerWidget::TimerStopped()
{
	bActive = false;
	UpdateCountTime(0.f); 
	if (bHiddenWhenInactive)
	{
		if (TextBlock_Time)
		{
			TextBlock_Time->SetRenderOpacity(0.f);
		}
	}
}

void UTimerWidget::UpdateCountTime(float TimeSeconds)
{
	InternalCountTime = TimeSeconds;
	const FText TimeText = FText::FromString(FormatTimeAsString(InternalCountTime));
	TextBlock_Time->SetText(TimeText);
}