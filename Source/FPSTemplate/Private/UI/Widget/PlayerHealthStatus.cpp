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
		
		/* Bind Overlay Widget Controller Delegates to Callback Functions */
		OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentHealthStatus); 
		OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxHealthStatus); 
		OverlayWidgetController->OnArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentArmorStatus); 
		OverlayWidgetController->OnMaxArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxArmorStatus);
		OverlayWidgetController->OnShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateCurrentShieldStatus);
		OverlayWidgetController->OnMaxShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateMaxShieldStatus);
		OverlayWidgetController->OnTempArmorChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateTempArmorStatus);
		OverlayWidgetController->OnTempShieldChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateTempShieldStatus);
		OverlayWidgetController->OnOverHealthChanged.AddDynamic(this, &UPlayerHealthStatus::UpdateOverHealthStatus);
	}

	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 
	/* Current Health Status */
	CurrentHealthStatusMap.Add(GameplayTags.Attributes_Defense_Health, 0.f); 
	CurrentHealthStatusMap.Add(GameplayTags.Attributes_Defense_Armor, 0.f); 
	CurrentHealthStatusMap.Add(GameplayTags.Attributes_Defense_Shield, 0.f); 
	CurrentHealthStatusMap.Add(GameplayTags.Attributes_Defense_TempArmor, 0.f);
	CurrentHealthStatusMap.Add(GameplayTags.Attributes_Defense_TempShield, 0.f);
	CurrentHealthStatusMap.Add(GameplayTags.Attributes_Defense_OverHealth, 0.f);

	/* Max Health Status */
	MaxHealthStatusMap.Add(GameplayTags.Attributes_Defense_MaxHealth, 0.f);
	MaxHealthStatusMap.Add(GameplayTags.Attributes_Defense_MaxArmor, 0.f);
	MaxHealthStatusMap.Add(GameplayTags.Attributes_Defense_MaxShield, 0.f); 
	MaxHealthStatusMap.Add(GameplayTags.Attributes_Defense_TempArmor, 0.f); 
	MaxHealthStatusMap.Add(GameplayTags.Attributes_Defense_TempShield, 0.f); 
	MaxHealthStatusMap.Add(GameplayTags.Attributes_Defense_OverHealth, 0.f); 
}

void UPlayerHealthStatus::SetCurrentHealth()
{
	CurrentHealth = 0.f; 
	for (TPair<FGameplayTag, float>& Pair : CurrentHealthStatusMap)
	{
		CurrentHealth += Pair.Value; 
	}
	TextBlock_CurrentHealth->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(CurrentHealth)));
}

void UPlayerHealthStatus::SetMaxHealth()
{
	MaxHealth = 0.f; 
	for (TPair<FGameplayTag, float>& Pair : MaxHealthStatusMap)
	{
		MaxHealth += Pair.Value;
	}
	TextBlock_MaxHealth->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(MaxHealth)));
}

void UPlayerHealthStatus::UpdateCurrentHealthStatus(float NewValue)
{
	if (NewValue == 0.f) return; 
	CurrentHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_Health] = NewValue; 
	SetCurrentHealth(); 
}

void UPlayerHealthStatus::UpdateMaxHealthStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	MaxHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_MaxHealth] = NewValue;
	SetMaxHealth(); 
}

void UPlayerHealthStatus::UpdateCurrentArmorStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	CurrentHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_Armor] = NewValue;
	SetCurrentHealth();
}

void UPlayerHealthStatus::UpdateMaxArmorStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	MaxHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_MaxArmor] = NewValue;
	SetMaxHealth();
}

void UPlayerHealthStatus::UpdateCurrentShieldStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	CurrentHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_Shield] = NewValue;
	SetCurrentHealth();
}

void UPlayerHealthStatus::UpdateMaxShieldStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	MaxHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_MaxShield] = NewValue;
	SetMaxHealth();
}

void UPlayerHealthStatus::UpdateTempArmorStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	CurrentHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_TempArmor] = NewValue;
	MaxHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_TempArmor] = NewValue;
	SetCurrentHealth();
	SetMaxHealth(); 
}

void UPlayerHealthStatus::UpdateTempShieldStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	CurrentHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_TempShield] = NewValue;
	MaxHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_TempShield] = NewValue;
	SetCurrentHealth();
	SetMaxHealth();
}

void UPlayerHealthStatus::UpdateOverHealthStatus(float NewValue)
{
	if (NewValue == 0.f) return;
	CurrentHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_OverHealth] = NewValue;
	MaxHealthStatusMap[FOWGameplayTags::Get().Attributes_Defense_OverHealth] = NewValue;
	SetCurrentHealth();
	SetMaxHealth();
}


