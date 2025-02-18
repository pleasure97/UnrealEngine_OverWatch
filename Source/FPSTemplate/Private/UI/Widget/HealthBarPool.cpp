// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBarPool.h"
#include "Algo/Copy.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "OWGameplayTags.h"
#include "UI/Widget/HealthBar.h"
#include "AbilitySystem/Data/HealthBarInfo.h"
#include "Kismet/KismetMathLibrary.h"

void UHealthBarPool::NativePreConstruct()
{
	Super::NativePreConstruct(); 

	// In FHealthBarInfo Constructor, CanAddOrRemoveHealthBar - true or false
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_MaxHealth, FHealthBarPoolInfo(Border_Health, HorizontalBox_Health, true));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_MaxArmor, FHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, true));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_MaxShield, FHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, true));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_OverHealth, FHealthBarPoolInfo(Border_OverHealth, HorizontalBox_OverHealth, true));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_TempArmor, FHealthBarPoolInfo(Border_TempArmor, HorizontalBox_TempArmor, true));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_TempShield, FHealthBarPoolInfo(Border_TempShield, HorizontalBox_TempShield, true));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_Health, FHealthBarPoolInfo(Border_Health, HorizontalBox_Health, false));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_Armor, FHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, false));
	TagsToHealthBarInfos.Add(FOWGameplayTags::Get().Attributes_Defense_Shield, FHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, false));
}

void UHealthBarPool::NativeConstruct()
{
	Super::NativeConstruct();

	check(WidgetController);

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController);
		OverlayWidgetController->OnUpdateHealthBars.AddDynamic(this, &UHealthBarPool::UpdateProgressBars);
	}
}

TArray<FHealthBarPoolInfo> UHealthBarPool::GetValidHealthBarInfos()
{
	TArray<FHealthBarPoolInfo> HealthBarInfos;
	for (const TPair<FGameplayTag, FHealthBarPoolInfo>& TagToHealthBarInfo: TagsToHealthBarInfos)
	{
		if (TagToHealthBarInfo.Value.CanAddOrRemoveHealthBar)
		{
			HealthBarInfos.Add(TagToHealthBarInfo.Value); 
		}
	}

	return HealthBarInfos; 
}


void UHealthBarPool::UpdateBorderVisibility()
{
	TArray<FHealthBarPoolInfo> HealthBarInfos = GetValidHealthBarInfos(); 

	for (FHealthBarPoolInfo& HealthBarInfo : HealthBarInfos)
	{
		UHorizontalBox* HorizontalBox = HealthBarInfo.HorizontalBox;
		UBorder* Border = HealthBarInfo.Border;

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
	TArray<FHealthBarPoolInfo> HealthBarInfos = GetValidHealthBarInfos();

	int32 NumAllChildren = 0; 

	for (const FHealthBarPoolInfo& HealthBarInfo : HealthBarInfos)
	{
		UHorizontalBox* HorizontalBox = HealthBarInfo.HorizontalBox;
		NumAllChildren += HorizontalBox->GetChildrenCount(); 
	}

	FSlateChildSize ChildSize;
	ChildSize.SizeRule = ESlateSizeRule::Fill;

	for (const FHealthBarPoolInfo& HealthBarInfo : HealthBarInfos)
	{
		UHorizontalBox* HorizontalBox = HealthBarInfo.HorizontalBox;
		UBorder* Border = HealthBarInfo.Border;

		int32 NumChildren = HorizontalBox->GetChildrenCount();
		float SlotSize = (float)NumChildren / (float)NumAllChildren;

		ChildSize.Value = SlotSize;

		if (UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Border->Slot))
		{
			HorizontalBoxSlot->SetSize(ChildSize);
		}
	}
}

void UHealthBarPool::UpdateProgressBars(const FBarInfo& Info)
{
	// Find the Horizontal Box related to received AttributeDefensiveInfo 
	if (!TagsToHealthBarInfos.Contains(Info.DefensiveAttributeTag)) return; 

	const FHealthBarPoolInfo& HealthBarInfo = TagsToHealthBarInfos[Info.DefensiveAttributeTag];
	if (HealthBarInfo.CanAddOrRemoveHealthBar)
	{
		UHorizontalBox* HorizontalBox = HealthBarInfo.HorizontalBox;

		// Calculate how many health bars should be added to the horizontal box 
		int32 NumHealthBars = FMath::CeilToInt(Info.AttributeValue / HealthPerBar);

		// Preset the size of the health bar 
		FSlateChildSize SlateChildSize;
		SlateChildSize.SizeRule = ESlateSizeRule::Fill;

		// Create the health bars and fill them in the horizontal box. 
		for (int i = 0; i < NumHealthBars; ++i)
		{
			UHealthBar* HealthBar = CreateWidget<UHealthBar>(this, HealthBarClass);

			HealthBar->UpdateProgressBar(Info.Tint_Fill, 1.f);

			UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBox->AddChildToHorizontalBox(HealthBar);
			HorizontalBoxSlot->SetSize(SlateChildSize);
		}

		UpdateBorderVisibility();

		DistributeFillSize();
	}
	else
	{
		float Remainder = 0;
		const int32 NumBarsToFill = UKismetMathLibrary::FMod(Info.AttributeValue, HealthPerBar, Remainder);
		const int32 NumChildren = HealthBarInfo.HorizontalBox->GetChildrenCount(); 

		for (int i = 0; i < NumChildren; ++i)
		{
			UHealthBar* HealthBar = Cast<UHealthBar>(HealthBarInfo.HorizontalBox->GetChildAt(i)); 
			
			if (i < NumBarsToFill)
			{
				HealthBar->UpdateProgressBar(Info.Tint_Fill, 1.f);
			}
			else if (i == NumBarsToFill)
			{
				HealthBar->UpdateProgressBar(Info.Tint_Fill, Remainder / HealthPerBar);
			}
			else
			{
				HealthBar->UpdateProgressBar(Info.Tint_Fill, 0.f); 
			}
			
		}
	}
}
