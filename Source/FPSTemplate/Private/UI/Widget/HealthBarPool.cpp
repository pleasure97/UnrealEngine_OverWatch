// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBarPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "OWGameplayTags.h"
#include "UI/Widget/HealthBar.h"
#include "AbilitySystem/Data/DefensiveAttributeInfo.h"

void UHealthBarPool::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_MaxHealth, FBorderAndHorizontalBox(Border_Health, HorizontalBox_Health));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_MaxArmor, FBorderAndHorizontalBox(Border_Armor, HorizontalBox_Armor));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_TempArmor, FBorderAndHorizontalBox(Border_TempArmor, HorizontalBox_TempArmor));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_MaxShield, FBorderAndHorizontalBox(Border_Shield, HorizontalBox_Shield));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_TempShield, FBorderAndHorizontalBox(Border_TempShield, HorizontalBox_TempShield));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_OverHealth, FBorderAndHorizontalBox(Border_OverHealth, HorizontalBox_OverHealth));

	UpdateBorderVisibility();

	DistributeFillSize(); 
}

void UHealthBarPool::UpdateBorderVisibility()
{
	TArray<FBorderAndHorizontalBox> BorderAndHorziontalBox;
	TagsToBorderAndHorizontalBox.GenerateValueArray(BorderAndHorziontalBox);

	for (FBorderAndHorizontalBox& Pair : BorderAndHorziontalBox)
	{
		UHorizontalBox* HorizontalBox = Pair.HorizontalBox;
		UBorder* Border = Pair.Border;

		if (HorizontalBox->GetChildrenCount() > 0)
		{
			Border->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Border->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UHealthBarPool::DistributeFillSize()
{
	TArray<FBorderAndHorizontalBox> BorderAndHorziontalBox;
	TagsToBorderAndHorizontalBox.GenerateValueArray(BorderAndHorziontalBox);

	int32 NumAllChildrens = 0; 

	for (const FBorderAndHorizontalBox& Pair : BorderAndHorziontalBox)
	{
		UHorizontalBox* HorizontalBox = Pair.HorizontalBox;
		NumAllChildrens += HorizontalBox->GetChildrenCount(); 
	}

	if (NumAllChildrens > 0)
	{
		FSlateChildSize ChildSize;
		ChildSize.SizeRule = ESlateSizeRule::Fill;

		for (const FBorderAndHorizontalBox& Pair : BorderAndHorziontalBox)
		{
			UHorizontalBox* HorizontalBox = Pair.HorizontalBox; 
			UBorder* Border = Pair.Border; 

			int32 NumChildrens = HorizontalBox->GetChildrenCount(); 
			float SlotSize = NumChildrens / (float)NumAllChildrens; 

			ChildSize.Value = SlotSize;

			if (UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Border->Slot))
			{
				HorizontalBoxSlot->SetSize(ChildSize); 
			}
		}
	}
}

void UHealthBarPool::NativeConstruct()
{
	Super::NativeConstruct();

	check(WidgetController);

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 
		OverlayWidgetController->AttributeInfoDelegate.AddDynamic(this, &UHealthBarPool::UpdateProgressBars);
	}

	TArray<FBorderAndHorizontalBox> BordersAndHorziontalBoxes; 
	TagsToBorderAndHorizontalBox.GenerateValueArray(BordersAndHorziontalBoxes); 

	for (FBorderAndHorizontalBox& BorderAndHorizontalBox : BordersAndHorziontalBoxes)
	{
		UHorizontalBox* HorizontalBox = BorderAndHorizontalBox.HorizontalBox;
		UBorder* Border = BorderAndHorizontalBox.Border;

		if (HorizontalBox->GetChildrenCount() > 0)
		{
			Border->SetVisibility(ESlateVisibility::HitTestInvisible); 
		}
		else
		{
			Border->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}
}

void UHealthBarPool::UpdateProgressBars(const FAttributeDefensiveInfo& Info)
{
	if (!Info.DefensiveAttributeTag.IsValid() || Info.DefensiveAttributeTag == FOWGameplayTags::Get().Attributes_Defense_Health) return;

	// Find the Horizontal Box related to received AttributeDefensiveInfo 
	const FBorderAndHorizontalBox& BorderAndHorizontalBox = TagsToBorderAndHorizontalBox[Info.DefensiveAttributeTag];
	UHorizontalBox* HorizontalBox = BorderAndHorizontalBox.HorizontalBox; 

	// Calculate how many health bars should be added to the horizontal box 
	int32 NumHealthBars = FMath::CeilToInt(Info.AttributeValue / HealthPerBar); 

	// Preset the size of the health bar 
	FSlateChildSize SlateChildSize; 
	SlateChildSize.SizeRule = ESlateSizeRule::Fill; 

	// Create the health bars and fill them in the horizontal box. 
	for (int i = 0; i < NumHealthBars; ++i)
	{
		UHealthBar* HealthBar = CreateWidget<UHealthBar>(this, HealthBarClass);

		if (FMath::Fmod(Info.AttributeValue, HealthPerBar) != 0.f && (i == NumHealthBars - 1))
		{
			float PercentValue = (Info.AttributeValue - NumHealthBars * HealthPerBar) / HealthPerBar;
			HealthBar->UpdateProgressBar(Info.Tint_Fill, PercentValue);
		}
		else
		{
			HealthBar->UpdateProgressBar(Info.Tint_Fill, 1.f);
		}
		UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBox->AddChildToHorizontalBox(HealthBar);
		HorizontalBoxSlot->SetSize(SlateChildSize); 
	}
	
	UpdateBorderVisibility(); 

	DistributeFillSize(); 
}
