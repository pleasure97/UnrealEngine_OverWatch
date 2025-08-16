// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Respawn/RespawnGauge.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URespawnGauge::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (Image_RespawnGauge)
	{
		MID_RespawnGauge = Image_RespawnGauge->GetDynamicMaterial();
	}
}

void URespawnGauge::NativeDestruct()
{
	if (MID_RespawnGauge)
	{
		MID_RespawnGauge = nullptr; 
	}

	Super::NativeDestruct();
}

void URespawnGauge::InitializeRespawnGauge(float RespawnWaitingDuration)
{
	if (UWorld* World = GetWorld())
	{
		StartTime = World->GetTimeSeconds();
		Duration = RespawnWaitingDuration;
		UpdateRespawnGauge(); 
		World->GetTimerManager().SetTimer(RespawnWaitingTimerHandle, this, &URespawnGauge::UpdateRespawnGauge, 0.1f, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returns nullptr in URespawnGauge::InitializeRespawnGauge()")); 
	}
}

void URespawnGauge::FinishTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RespawnWaitingTimerHandle); 
	}
}

void URespawnGauge::UpdateRespawnGauge()
{
	if (UWorld* World = GetWorld())
	{
		TimeUntilRespawn = (StartTime + Duration) - World->GetTimeSeconds();
		if (TimeUntilRespawn > 0.f)
		{
			if (MID_RespawnGauge)
			{
				float Percent = TimeUntilRespawn / Duration;
				MID_RespawnGauge->SetScalarParameterValue(TEXT("Percent"), Percent); 
			}

			if (TextBlock_RespawnTime)
			{
				int32 DisplayTime = FMath::FloorToInt(TimeUntilRespawn);
				FText DisplayText = FText::AsNumber(DisplayTime);
				TextBlock_RespawnTime->SetText(DisplayText); 
			}
		}
	}
}
