// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHealthStatus.h"
#include "OWGameplayTags.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerHealthStatus::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(WidgetController); 
		
		OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus); 
		OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus); 
		OverlayWidgetController->OnArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus); 
		OverlayWidgetController->OnMaxArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus);
		OverlayWidgetController->OnShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus);
		OverlayWidgetController->OnMaxShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus);
		OverlayWidgetController->OnTempArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus);
		OverlayWidgetController->OnTempArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus);
		OverlayWidgetController->OnTempShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus);
		OverlayWidgetController->OnTempShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus);
		OverlayWidgetController->OnOverHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus);
		OverlayWidgetController->OnOverHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus);
	}
}

void UPlayerHealthStatus::UpdateCurrentHealthStatus(float NewValue)
{
	CurrentHealth += NewValue; 
	TextBlock_CurrentHealth->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(CurrentHealth))); 
}

void UPlayerHealthStatus::UpdateMaxHealthStatus(float NewValue)
{
	MaxHealth += NewValue; 
	TextBlock_MaxHealth->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(MaxHealth)));
}


