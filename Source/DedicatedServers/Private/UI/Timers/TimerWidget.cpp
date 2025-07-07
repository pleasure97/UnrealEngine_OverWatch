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
		if (TextBlock_Time)
		{
			TextBlock_Time->SetVisibility(ESlateVisibility::Collapsed); 
		}
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
void UTimerWidget::NativeDestruct()
{
	OwningPlayerController = Cast<ADSPlayerController>(GetOwningPlayer());
	if (IsValid(OwningPlayerController))
	{
		OwningPlayerController->OnTimerUpdated.RemoveAll(this);
		OwningPlayerController->OnTimerStopped.RemoveAll(this);
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
	if (TimeSeconds < 0.f)
	{
		TimeSeconds = 0.f; 
	}

	// Format Example - 4:00 ~ 0:00
	if (bAlwaysShowMinutes)
	{
		const int32 Min = FMath::FloorToInt(TimeSeconds / 60.f);
		const int32 Sec = FMath::FloorToInt(TimeSeconds) % 60;
		return FString::Printf(TEXT("%d:%02d"), Min, Sec);
	}

	// Format Example - 0:45 ~ 0:10 -> 10.0 -> 0.0
	if (bShowDecimalIfUnderTenSeconds && TimeSeconds < 10.f)
	{
		const float Rounded = FMath::RoundToFloat(TimeSeconds * 10.f) / 10.f;
		return FString::Printf(TEXT("%.1f"), Rounded);
	}

	// Format Example  - 45 -> 0 
	if (!bShowDecimalIfUnderTenSeconds)
	{
		const int32 WholeSec = FMath::FloorToInt(TimeSeconds);
		return FString::Printf(TEXT("%d"), WholeSec);
	}

	// Format Example - 4:00 ~ 0:00
	const int32 Min = FMath::FloorToInt(TimeSeconds / 60.f);
	const int32 Sec = FMath::FloorToInt(TimeSeconds) % 60;
	return FString::Printf(TEXT("%d:%02d"), Min, Sec);
}

void UTimerWidget::TimerStarted(float InitialTime)
{
	bActive = true; 
	if (TextBlock_Time)
	{
		TextBlock_Time->SetVisibility(ESlateVisibility::Visible); 
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
			TextBlock_Time->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}
}

void UTimerWidget::UpdateCountTime(float TimeSeconds)
{
	InternalCountTime = TimeSeconds;
	const FText TimeText = FText::FromString(FormatTimeAsString(InternalCountTime));
	if (TextBlock_Time)
	{
		TextBlock_Time->SetText(TimeText);
	}
}