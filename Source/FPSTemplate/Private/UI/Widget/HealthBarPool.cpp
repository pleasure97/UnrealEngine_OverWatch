// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBarPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/HorizontalBox.h"
#include "OWGameplayTags.h"
#include "UI/Widget/HealthBar.h"
#include "AbilitySystem/Data/DefensiveAttributeInfo.h"
#include "Components/SizeBox.h"

void UHealthBarPool::NativeConstruct()
{
	Super::NativeConstruct();

	check(WidgetController);

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 
		OverlayWidgetController->AttributeInfoDelegate.AddDynamic(this, &UHealthBarPool::UpdateProgressBars);
	}

	TagsToHorizontalBoxes.Add(FOWGameplayTags::Get().Attributes_Defense_MaxHealth, HorizontalBox_Health); 
	TagsToHorizontalBoxes.Add(FOWGameplayTags::Get().Attributes_Defense_Armor, HorizontalBox_Armor);
	TagsToHorizontalBoxes.Add(FOWGameplayTags::Get().Attributes_Defense_Shield, HorizontalBox_Shield);
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
			HorizontalBoxToUpdate->AddChildToHorizontalBox(NewHealthBar);
		}

		UpdateHorizontalBoxSize(); 

		
		HorizontalBoxToUpdate->getchild

	}
}

void UHealthBarPool::UpdateHorizontalBoxSize()
{
	TotalNumBars = HorizontalBox_Health->GetChildrenCount() + HorizontalBox_Armor->GetChildrenCount() + HorizontalBox_Shield->GetChildrenCount();
}
