// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHealthStatus.h"
#include "OWGameplayTags.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/DefensiveAttributeInfo.h"
#include "Components/TextBlock.h"

void UPlayerHealthStatus::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	/* Current Health */
	TagsToTextBlocks.Add(FOWGameplayTags::Get().Attributes_Defense_Health, TextBlock_CurrentHealth );
	TagsToTextBlocks.Add(FOWGameplayTags::Get().Attributes_Defense_Armor, TextBlock_CurrentHealth);
	TagsToTextBlocks.Add(FOWGameplayTags::Get().Attributes_Defense_Shield, TextBlock_CurrentHealth);

	/* Max Health */
	TagsToTextBlocks.Add(FOWGameplayTags::Get().Attributes_Defense_MaxHealth, TextBlock_MaxHealth);
	TagsToTextBlocks.Add(FOWGameplayTags::Get().Attributes_Defense_MaxArmor, TextBlock_MaxHealth); 
	TagsToTextBlocks.Add(FOWGameplayTags::Get().Attributes_Defense_MaxShield, TextBlock_MaxHealth); 
}

void UPlayerHealthStatus::NativeConstruct()
{
	Super::NativeConstruct(); 

	check(WidgetController); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(WidgetController); 
		OverlayWidgetController->AttributeInfoDelegate.AddDynamic(this, &UPlayerHealthStatus::UpdatePlayerStatus); 
	}
}

void UPlayerHealthStatus::UpdatePlayerStatus(const FAttributeDefensiveInfo& Info)
{
	if (!TagsToTextBlocks.Contains(Info.DefensiveAttributeTag)) return; 

	if (TagsToTextBlocks[Info.DefensiveAttributeTag] == TextBlock_CurrentHealth)
	{
		CurrentHealth += Info.AttributeValue; 
		int32 FlooredCurrentHealth = FMath::FloorToInt(CurrentHealth); 
		TextBlock_CurrentHealth->SetText(FText::AsNumber(FlooredCurrentHealth)); 
	}
	
	if (TagsToTextBlocks[Info.DefensiveAttributeTag] == TextBlock_MaxHealth)
	{
		MaxHealth += Info.AttributeValue;
		int32 FlooredMaxHealth = FMath::FloorToInt(MaxHealth);
		TextBlock_MaxHealth->SetText(FText::AsNumber(FlooredMaxHealth));
	}
}

