// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBarPool.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "OWGameplayTags.h"
#include "UI/Widget/HealthBar.h"
#include "Character/OWCharacterBase.h"
#include "Actor/HealingSunStone.h"

void UHealthBarPool::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeHealthBarPoolInfos();

	BindWidgetControllerEvents();
}

void UHealthBarPool::InitializeHealthBarPoolInfos()
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxHealth, FHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors::White));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxArmor, FHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, HealthBarColors::Orange));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxShield, FHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, HealthBarColors::Sky));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_OverHealth, FHealthBarPoolInfo(Border_OverHealth, HorizontalBox_OverHealth, HealthBarColors::Green));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_TempArmor, FHealthBarPoolInfo(Border_TempArmor, HorizontalBox_TempArmor, HealthBarColors::Orange));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_TempShield, FHealthBarPoolInfo(Border_TempShield, HorizontalBox_TempShield, HealthBarColors::Blue));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Health, FHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors::White));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Armor, FHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, HealthBarColors::Orange));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Shield, FHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, HealthBarColors::Sky));

	HealthBarInfos.Add(FHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors::White));
	HealthBarInfos.Add(FHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, HealthBarColors::Orange));
	HealthBarInfos.Add(FHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, HealthBarColors::None));
	HealthBarInfos.Add(FHealthBarPoolInfo(Border_OverHealth, HorizontalBox_OverHealth, HealthBarColors::Green));
	HealthBarInfos.Add(FHealthBarPoolInfo(Border_TempArmor, HorizontalBox_TempArmor, HealthBarColors::Orange));
	HealthBarInfos.Add(FHealthBarPoolInfo(Border_TempShield, HorizontalBox_TempShield, HealthBarColors::Blue));
}

void UHealthBarPool::BindWidgetControllerEvents()
{
	if (TagsToHealthBarInfos.IsEmpty() || HealthBarInfos.IsEmpty())
	{
		InitializeHealthBarPoolInfos(); 
	}

	// TODO - Change Repetitive and Inefficient Codes 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController);

		OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxHealthBars);
		OverlayWidgetController->OnMaxArmorChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxArmorBars);
		OverlayWidgetController->OnMaxShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxShieldBars);
		OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateHealthBars);
		OverlayWidgetController->OnArmorChanged.AddDynamic(this, &UHealthBarPool::UpdateArmorBars);
		OverlayWidgetController->OnShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateShieldBars);
		OverlayWidgetController->OnTempArmorChanged.AddDynamic(this, &UHealthBarPool::UpdateTempArmorBars);
		OverlayWidgetController->OnTempShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateTempShieldBars);
		OverlayWidgetController->OnOverHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateOverHealthBars);
	}

	if (AOWCharacterBase* OWCharacterBase = Cast<AOWCharacterBase>(WidgetController))
	{
		SetWidgetController(OWCharacterBase);

		OWCharacterBase->OnMaxHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxHealthBars);
		OWCharacterBase->OnMaxArmorChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxArmorBars);
		OWCharacterBase->OnMaxShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxShieldBars);
		OWCharacterBase->OnHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateHealthBars);
		OWCharacterBase->OnArmorChanged.AddDynamic(this, &UHealthBarPool::UpdateArmorBars);
		OWCharacterBase->OnShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateShieldBars);
		OWCharacterBase->OnTempArmorChanged.AddDynamic(this, &UHealthBarPool::UpdateTempArmorBars);
		OWCharacterBase->OnTempShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateTempShieldBars);
		OWCharacterBase->OnOverHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateOverHealthBars);
	}

	if (AHealingSunStone* HealingSunStone = Cast<AHealingSunStone>(WidgetController))
	{
		HealingSunStone->OnMaxHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxHealthBars);
		HealingSunStone->OnMaxShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateMaxShieldBars);
		HealingSunStone->OnHealthChanged.AddDynamic(this, &UHealthBarPool::UpdateHealthBars);
		HealingSunStone->OnShieldChanged.AddDynamic(this, &UHealthBarPool::UpdateShieldBars);
	}
}

void UHealthBarPool::InitializeProgressBars(const float& NewValue, const FHealthBarPoolInfo& HealthBarPoolInfo)
{
	UHorizontalBox* HorizontalBox = HealthBarPoolInfo.HorizontalBox;

	// Calculate how many health bars should be added to the horizontal box 
	int32 NumHealthBars = FMath::CeilToInt(NewValue / HealthPerBar);

	// Preset the size of the health bar 
	FSlateChildSize SlateChildSize;
	SlateChildSize.SizeRule = ESlateSizeRule::Fill;

	// Create the health bars and fill them in the horizontal box. 
	for (int i = 0; i < NumHealthBars; ++i)
	{
		UHealthBar* HealthBar = CreateWidget<UHealthBar>(this, HealthBarClass);
		HealthBar->UpdateProgressBar(HealthBarPoolInfo.HealthBarColor, 1.f);

		UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBox->AddChildToHorizontalBox(HealthBar);
		HorizontalBoxSlot->SetSize(SlateChildSize);
	}
}

void UHealthBarPool::UpdateProgressBars(const float& NewValue, const FHealthBarPoolInfo& HealthBarInfo)
{
	const int32 NumBarsToFill = FMath::FloorToInt(NewValue / HealthPerBar);
	const float Remainder = FMath::Fmod(NewValue, HealthPerBar);
	const int32 NumChildren = HealthBarInfo.HorizontalBox->GetChildrenCount();

	for (int i = 0; i < NumChildren; ++i)
	{
		UHealthBar* HealthBar = Cast<UHealthBar>(HealthBarInfo.HorizontalBox->GetChildAt(i));

		if (i < NumBarsToFill)
		{
			HealthBar->UpdateProgressBar(HealthBarInfo.HealthBarColor, 1.f);
		}
		else if (i == NumBarsToFill)
		{
			HealthBar->UpdateProgressBar(HealthBarInfo.HealthBarColor, Remainder / HealthPerBar);
		}
		else
		{
			HealthBar->UpdateProgressBar(HealthBarInfo.HealthBarColor, 0.f);
		}

	}
}

void UHealthBarPool::UpdateMaxHealthBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_MaxHealth];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility(); 

	DistributeFillSize(); 
}

void UHealthBarPool::UpdateMaxArmorBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_MaxArmor];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UHealthBarPool::UpdateMaxShieldBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_MaxShield];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UHealthBarPool::UpdateHealthBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_Health]; 
	if (HealthBarPoolInfo.HorizontalBox->GetChildrenCount() == 0) return; 

	UpdateProgressBars(NewValue, HealthBarPoolInfo); 
}

void UHealthBarPool::UpdateArmorBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_Armor];
	if (HealthBarPoolInfo.HorizontalBox->GetChildrenCount() == 0) return;

	UpdateProgressBars(NewValue, HealthBarPoolInfo);
}

void UHealthBarPool::UpdateShieldBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_Shield];
	if (HealthBarPoolInfo.HorizontalBox->GetChildrenCount() == 0) return;

	UpdateProgressBars(NewValue, HealthBarPoolInfo);
}

void UHealthBarPool::UpdateTempArmorBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_TempArmor];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UHealthBarPool::UpdateTempShieldBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_TempShield];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UHealthBarPool::UpdateOverHealthBars(float NewValue)
{
	const FHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_OverHealth];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}


void UHealthBarPool::UpdateBorderVisibility()
{
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