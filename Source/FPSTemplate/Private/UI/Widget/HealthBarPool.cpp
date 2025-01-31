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
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_Armor, FBorderAndHorizontalBox(Border_Armor, HorizontalBox_Armor));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_TempArmor, FBorderAndHorizontalBox(Border_TempArmor, HorizontalBox_TempArmor));
	TagsToBorderAndHorizontalBox.Add(FOWGameplayTags::Get().Attributes_Defense_Shield, FBorderAndHorizontalBox(Border_Shield, HorizontalBox_Shield));
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

	TArray<FBorderAndHorizontalBox> BorderAndHorziontalBox; 
	TagsToBorderAndHorizontalBox.GenerateValueArray(BorderAndHorziontalBox); 

	for (FBorderAndHorizontalBox& Pair: BorderAndHorziontalBox)
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

void UHealthBarPool::UpdateProgressBars(const FAttributeDefensiveInfo& Info)
{
	if (bNotInitialized)
	{
		InitializePool(Info); 
		bNotInitialized = false; 
	}

	if (TObjectPtr<UHorizontalBox>* HorizontalBoxPtr = TagsToHorizontalBoxes.Find(Info.DefensiveAttributeTag))
	{
		UHorizontalBox* HorizontalBoxToUpdate = *HorizontalBoxPtr; 

		for (UWidget* ChildWidget : HorizontalBoxToUpdate->GetAllChildren())
		{
			if (UHealthBar* HealthBar = Cast<UHealthBar>(ChildWidget))
			{
				HealthBar->UpdateProgressBar(Info.Tint_Fill, Info.AttributeValue); 
			}
		}
	}
}

void UHealthBarPool::InitializePool(const FAttributeDefensiveInfo& Info)
{
	const FGameplayTag& DefensiveAttributeTag = Info.DefensiveAttributeTag; 
	const float AttributeValue = Info.AttributeValue; 

	if (DefensiveAttributeTag.MatchesTagExact(FOWGameplayTags::Get().Attributes_Defense_Health)) return; 

	const int32 NumHealthBars = FMath::CeilToInt(AttributeValue / HealthPerBar); 

	if (TObjectPtr<UHorizontalBox>* HorizontalBoxPtr = TagsToHorizontalBoxes.Find(Info.DefensiveAttributeTag))
	{
		UHorizontalBox* HorizontalBoxToUpdate = *HorizontalBoxPtr;

		for (int32 i = 0; i < NumHealthBars; ++i)
		{
			UHealthBar* NewHealthBar = CreateWidget<UHealthBar>(this, HealthBarClass);
			HorizontalBoxToUpdate->AddChild(NewHealthBar);
		}

		UpdateHorizontalBoxSize(); 
	}
}

void UHealthBarPool::UpdateHorizontalBoxSize()
{
	TotalNumBars = HorizontalBox_Health->GetChildrenCount() + HorizontalBox_Armor->GetChildrenCount() + HorizontalBox_Shield->GetChildrenCount();

	const int32 HealthNumBars = HorizontalBox_Health->GetChildrenCount() / TotalNumBars;
	const int32 ArmorNumBars = HorizontalBox_Health->GetChildrenCount() / TotalNumBars;
	const int32 SheildNumBars = HorizontalBox_Health->GetChildrenCount() / TotalNumBars;
}
