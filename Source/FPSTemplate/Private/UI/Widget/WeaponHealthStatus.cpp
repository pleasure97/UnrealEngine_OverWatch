// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WeaponHealthStatus.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetTextLibrary.h"


void UWeaponHealthStatus::UpdateText(float Health)
{
	FText RemainingHealthText = UKismetTextLibrary::Conv_IntToText(FMath::TruncToInt(Health));

	TextBlock_WeaponHealthStatus->SetText(RemainingHealthText); 
}

void UWeaponHealthStatus::UpdatProgressBar(float HealthPercentage)
{
	ProgressBar_WeaponHealthStatus->SetPercent(HealthPercentage); 
}
