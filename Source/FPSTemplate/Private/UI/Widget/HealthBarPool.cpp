// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBarPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/HorizontalBox.h"
#include "UI/Widget/HealthBar.h"

void UHealthBarPool::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UHealthBarPool::NativeConstruct()
{
	Super::NativeConstruct();

	check(WidgetController);

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UHealthBarPool::OnHealthChanged);
		OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UHealthBarPool::OnMaxHealthChanged); 
		OverlayWidgetController->OnArmorChanged.AddDynamic(this, &UHealthBarPool::OnArmorChanged); 
		OverlayWidgetController->OnTempArmorChanged.AddDynamic(this, &UHealthBarPool::OnTempArmorChanged); 
		OverlayWidgetController->OnShieldChanged.AddDynamic(this, &UHealthBarPool::OnShieldChanged); 
		OverlayWidgetController->OnTempShieldChanged.AddDynamic(this, &UHealthBarPool::OnTempShieldChanged); 
		OverlayWidgetController->OnOverHealthChanged.AddDynamic(this, &UHealthBarPool::OnOverHealthChanged); 
	}

	//int RequiredBars = FMath::CeilToInt(CurrentHealth / HealthPerBar);

	//for (int i = 0; i < RequiredBars; ++i)
	//{

	//}
}

void UHealthBarPool::OnHealthChanged(float NewValue)
{
	
}

void UHealthBarPool::OnMaxHealthChanged(float NewValue)
{
}

void UHealthBarPool::OnArmorChanged(float NewValue)
{
}

void UHealthBarPool::OnTempArmorChanged(float NewValue)
{
}

void UHealthBarPool::OnShieldChanged(float NewValue)
{
}

void UHealthBarPool::OnTempShieldChanged(float NewValue)
{
}

void UHealthBarPool::OnOverHealthChanged(float NewValue)
{
}

void UHealthBarPool::UpdateProgressBars(float NewValue)
{
}
