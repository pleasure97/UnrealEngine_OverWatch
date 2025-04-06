// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHealthStatus.h"
#include "OWGameplayTags.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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

	CachedMPCInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
}

void UPlayerHealthStatus::UpdateStatus(FGameplayTag Tag, float NewValue, EHealthStatus HealthStatus)
{
	switch (HealthStatus)
	{
		case EHealthStatus::Current:
		{
			float& ValueToChange = CurrentHealthStatusMap[Tag]; 
			float OldValue = ValueToChange; 
			ValueToChange = NewValue; 

			CurrentHealth += (NewValue - OldValue); 
			CheckDamagedOrHealed(OldValue, NewValue); 
			TextBlock_CurrentHealth->SetText(FText::AsNumber(FMath::TruncToInt(CurrentHealth)));

			break; 
		}
		case EHealthStatus::Max:
		{
			float& ValueToChange = MaxHealthStatusMap[Tag];
			float OldValue = ValueToChange;
			ValueToChange = NewValue;

			MaxHealth += (NewValue - OldValue);
			TextBlock_MaxHealth->SetText(FText::AsNumber(FMath::TruncToInt(MaxHealth)));

			break; 
		}
		case EHealthStatus::All:
		{
			float& CurrentValueToChange = CurrentHealthStatusMap[Tag];
			float OldCurrentValue = CurrentValueToChange;
			CurrentValueToChange = NewValue;

			CurrentHealth += (NewValue - OldCurrentValue);
			CheckDamagedOrHealed(OldCurrentValue, NewValue);
			TextBlock_CurrentHealth->SetText(FText::AsNumber(FMath::TruncToInt(CurrentHealth)));

			float& MaxValueToChange = MaxHealthStatusMap[Tag];
			float OldMaxValue = MaxValueToChange;
			MaxValueToChange = NewValue; 

			MaxHealth += (NewValue - OldMaxValue);
			TextBlock_MaxHealth->SetText(FText::AsNumber(FMath::TruncToInt(MaxHealth)));

			break; 
		}
	}
}

void UPlayerHealthStatus::CheckDamagedOrHealed(float OldValue, float NewValue)
{
	if (!CachedMPCInstance) return; 

	// Damaged 
	// TODO - Consider How to Show Fatal State in Another Client's HUD 
	if (CurrentHealth < MaxHealth * FatalPercentage)
	{
		CachedMPCInstance->SetScalarParameterValue(TEXT("Damage"), 1.f); 
	}
	else
	{
		CachedMPCInstance->SetScalarParameterValue(TEXT("Damage"), 0.f); 
	}

	// Healed 
	if (OldValue < NewValue)
	{
		CachedMPCInstance->SetScalarParameterValue(TEXT("Healing"), 1.f); 

		GetWorld()->GetTimerManager().ClearTimer(HealingResetTimerHandle); 

		GetWorld()->GetTimerManager().SetTimer(HealingResetTimerHandle, this, &UPlayerHealthStatus::ResetHealingEffect, 1.f, false); 
	}
}

void UPlayerHealthStatus::ResetHealingEffect()
{
	if (CachedMPCInstance)
	{
		CachedMPCInstance->SetScalarParameterValue(TEXT("Healing"), 0.f); 
	}
}

void UPlayerHealthStatus::UpdateCurrentHealthStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_Health, NewValue, EHealthStatus::Current); 
}

void UPlayerHealthStatus::UpdateMaxHealthStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_MaxHealth, NewValue, EHealthStatus::Max);
}

void UPlayerHealthStatus::UpdateCurrentArmorStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_Armor, NewValue, EHealthStatus::Current);
}

void UPlayerHealthStatus::UpdateMaxArmorStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_MaxArmor, NewValue, EHealthStatus::Max);
}

void UPlayerHealthStatus::UpdateCurrentShieldStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_Shield, NewValue, EHealthStatus::Current);
}

void UPlayerHealthStatus::UpdateMaxShieldStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_MaxShield, NewValue, EHealthStatus::Max);
}

void UPlayerHealthStatus::UpdateTempArmorStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_TempArmor, NewValue, EHealthStatus::All);
}

void UPlayerHealthStatus::UpdateTempShieldStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_TempShield, NewValue, EHealthStatus::All);
}

void UPlayerHealthStatus::UpdateOverHealthStatus(float NewValue)
{
	UpdateStatus(FOWGameplayTags::Get().Attributes_Defense_OverHealth, NewValue, EHealthStatus::All);
}


