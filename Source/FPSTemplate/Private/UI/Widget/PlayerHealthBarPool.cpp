// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHealthBarPool.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "OWGameplayTags.h"
#include "UI/Widget/HealthBar.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemComponent.h"
#include "AbilitySystem/OWAttributeSet.h"

void UPlayerHealthBarPool::NativeConstruct()
{
	Super::NativeConstruct();

	ClearHealthBarPool();

	InitializeHealthBarPoolInfos();
}

void UPlayerHealthBarPool::NativeDestruct()
{
	// Remove Binding of Gameplay Attribute Value Change Delegate of Ability System Component 
	if (IsValid(OwnerAbilitySystemComponent) && IsValid(OwnerAttributeSet))
	{
		for (auto& TagToDefensiveAttribute : OwnerAttributeSet->TagsToDefensiveAttributes)
		{
			const FGameplayTag& DefensiveTag = TagToDefensiveAttribute.Key;
			const FGameplayAttribute& DefensiveAttribute = TagToDefensiveAttribute.Value();

			OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefensiveAttribute).RemoveAll(this);
		}
	}

	if (OWPlayerState)
	{
		OWPlayerState->GetTeamChangedDelegate().RemoveAll(this); 
	}

	ClearHealthBarPool();

	Super::NativeDestruct();
}

void UPlayerHealthBarPool::InitializeHealthBarPoolInfos()
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get();

	// Initialize the Mapping between Defensive Attribute Gameplay Tag and Health Bar Pool Info 
	// (Defensive Attribute Gameplay Tag - Border, Horizontal Box, Color, UpdateBars())
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxHealth,
		FPlayerHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors[FName("White")], &UPlayerHealthBarPool::UpdateMaxHealthBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxArmor,
		FPlayerHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, HealthBarColors[FName("Orange")], &UPlayerHealthBarPool::UpdateMaxArmorBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxShield,
		FPlayerHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, HealthBarColors[FName("Sky")], &UPlayerHealthBarPool::UpdateMaxShieldBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_OverHealth,
		FPlayerHealthBarPoolInfo(Border_OverHealth, HorizontalBox_OverHealth, HealthBarColors[FName("Green")], &UPlayerHealthBarPool::UpdateOverHealthBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_TempArmor,
		FPlayerHealthBarPoolInfo(Border_TempArmor, HorizontalBox_TempArmor, HealthBarColors[FName("Orange")], &UPlayerHealthBarPool::UpdateTempArmorBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_TempShield,
		FPlayerHealthBarPoolInfo(Border_TempShield, HorizontalBox_TempShield, HealthBarColors[FName("Blue")], &UPlayerHealthBarPool::UpdateTempShieldBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Health,
		FPlayerHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors[FName("White")], &UPlayerHealthBarPool::UpdateHealthBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Armor,
		FPlayerHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, HealthBarColors[FName("Orange")], &UPlayerHealthBarPool::UpdateArmorBars));
	TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Shield,
		FPlayerHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, HealthBarColors[FName("Sky")], &UPlayerHealthBarPool::UpdateShieldBars));

	HealthBarInfos.Add(FPlayerHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors[FName("White")]));
	HealthBarInfos.Add(FPlayerHealthBarPoolInfo(Border_Armor, HorizontalBox_Armor, HealthBarColors[FName("Orange")]));
	HealthBarInfos.Add(FPlayerHealthBarPoolInfo(Border_Shield, HorizontalBox_Shield, HealthBarColors[FName("Sky")]));
	HealthBarInfos.Add(FPlayerHealthBarPoolInfo(Border_OverHealth, HorizontalBox_OverHealth, HealthBarColors[FName("Green")]));
	HealthBarInfos.Add(FPlayerHealthBarPoolInfo(Border_TempArmor, HorizontalBox_TempArmor, HealthBarColors[FName("Orange")]));
	HealthBarInfos.Add(FPlayerHealthBarPoolInfo(Border_TempShield, HorizontalBox_TempShield, HealthBarColors[FName("Blue")]));
}

void UPlayerHealthBarPool::SetPlayerState(AOWPlayerState* NewOWPlayerState)
{
	if ((OWPlayerState == nullptr) && (NewOWPlayerState != nullptr))
	{
		OWPlayerState = NewOWPlayerState;

		BindDefensiveAttributeChange(NewOWPlayerState);
	}
}

void UPlayerHealthBarPool::SetAbilitySystemComponent(UOWAbilitySystemComponent* NewAbilitySystemComponent)
{
	BindDefensiveAttributeChangeWithASC(NewAbilitySystemComponent); 
}

void UPlayerHealthBarPool::SetIsEnemy(bool bEnemy)
{
	const FOWGameplayTags& GameplayTags = FOWGameplayTags::Get(); 

	if (TagsToHealthBarInfos.IsEmpty())
	{
		if (bEnemy)
		{
			TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_MaxHealth,
				FPlayerHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors[FName("Red")], &UPlayerHealthBarPool::UpdateMaxHealthBars));
			TagsToHealthBarInfos.Add(GameplayTags.Attributes_Defense_Health,
				FPlayerHealthBarPoolInfo(Border_Health, HorizontalBox_Health, HealthBarColors[FName("Red")], &UPlayerHealthBarPool::UpdateHealthBars));
		}
	}
	else
	{
		if (bEnemy)
		{
			if (FPlayerHealthBarPoolInfo* MaxHealthBarInfo = TagsToHealthBarInfos.Find(GameplayTags.Attributes_Defense_MaxHealth))
			{
				MaxHealthBarInfo->HealthBarColor = HealthBarColors[FName("Red")];
			}
			if (FPlayerHealthBarPoolInfo* HealthBarInfo = TagsToHealthBarInfos.Find(GameplayTags.Attributes_Defense_Health))
			{
				HealthBarInfo->HealthBarColor = HealthBarColors[FName("Red")];
			}
		}
	}
}

void UPlayerHealthBarPool::BindDefensiveAttributeChange(AOWPlayerState* NewOWPlayerState)
{
	// Cast Owning Player State to Custom Player State
	if (IsValid(NewOWPlayerState))
	{
		// Get Ability System Component from Custom Player State and Cast it to Custom Ability System Component
		if (UOWAbilitySystemComponent* InAbilitySystemComponent = Cast<UOWAbilitySystemComponent>(OWPlayerState->GetAbilitySystemComponent()))
		{
			// Assign Owner Ability System Component Member Variable 
			OwnerAbilitySystemComponent = InAbilitySystemComponent;
			// Get Attribute Set from Owner Ability System Component and Cast it to Custom Attribute Set
			if (UOWAttributeSet* InAttributeSet = const_cast<UOWAttributeSet*>(OwnerAbilitySystemComponent->GetSet<UOWAttributeSet>()))
			{
				// Assign Owner Attribute Set Member Variable 
				OwnerAttributeSet = InAttributeSet;
				// Check Owner Ability System Component and Attribute Set are Valid 
				if (IsValid(OwnerAbilitySystemComponent) && IsValid(OwnerAttributeSet))
				{
					// Iterate Defensive Attribute Map of Owner Attribute Set
					for (auto& TagToDefensiveAttribute : OwnerAttributeSet->TagsToDefensiveAttributes)
					{
						const FGameplayTag& DefensiveTag = TagToDefensiveAttribute.Key;
						const FGameplayAttribute& DefensiveAttribute = TagToDefensiveAttribute.Value();

						// Bind Gameplay Attribute Value Change Delegate of Owner Ability System Component
						OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefensiveAttribute).AddLambda(
							[this, DefensiveTag](const FOnAttributeChangeData& Data)
							{
								OnDefensiveAttributeChanged(DefensiveTag, Data.NewValue);
							}
						);
						// Call in advance to Initilaize Health Bar Pool 
						OnDefensiveAttributeChanged(DefensiveTag, DefensiveAttribute.GetNumericValue(OwnerAttributeSet));
					}
				}
			}
		}
	}
}

void UPlayerHealthBarPool::BindDefensiveAttributeChangeWithASC(UOWAbilitySystemComponent* NewAbilitySystemComponent)
{
	// Check if Ability System Component is Valid 
	if (IsValid(NewAbilitySystemComponent))
	{
		// Assign Owner Ability System Component Member Variable 
		OwnerAbilitySystemComponent = NewAbilitySystemComponent;
		// Get Attribute Set from Owner Ability System Component and Cast it to Custom Attribute Set
		if (UOWAttributeSet* InAttributeSet = const_cast<UOWAttributeSet*>(OwnerAbilitySystemComponent->GetSet<UOWAttributeSet>()))
		{
			// Assign Owner Attribute Set Member Variable 
			OwnerAttributeSet = InAttributeSet;
			// Check Owner Ability System Component and Attribute Set are Valid 
			if (IsValid(OwnerAbilitySystemComponent) && IsValid(OwnerAttributeSet))
			{
				// Iterate Defensive Attribute Map of Owner Attribute Set
				for (auto& TagToDefensiveAttribute : OwnerAttributeSet->TagsToDefensiveAttributes)
				{
					const FGameplayTag& DefensiveTag = TagToDefensiveAttribute.Key;
					const FGameplayAttribute& DefensiveAttribute = TagToDefensiveAttribute.Value();

					// Bind Gameplay Attribute Value Change Delegate of Owner Ability System Component
					OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DefensiveAttribute).AddLambda(
						[this, DefensiveTag](const FOnAttributeChangeData& Data)
						{
							OnDefensiveAttributeChanged(DefensiveTag, Data.NewValue);
						}
					);
					// Call in advance to Initilaize Health Bar Pool 
					OnDefensiveAttributeChanged(DefensiveTag, DefensiveAttribute.GetNumericValue(OwnerAttributeSet));
				}
			}
		}
	}
}

void UPlayerHealthBarPool::OnDefensiveAttributeChanged(FGameplayTag AttributeTag, float NewValue)
{
	if (TagsToHealthBarInfos.IsEmpty())
	{
		InitializeHealthBarPoolInfos(); 
	}

	if (FPlayerHealthBarPoolInfo* HealthBarPoolInfo = TagsToHealthBarInfos.Find(AttributeTag))
	{
		// e.g., AttributeTag = MaxHealth, ExecuteUpdate = UpdateMaxHealthBars()
		HealthBarPoolInfo->ExecuteUpdate(this, NewValue);
	}
}

void UPlayerHealthBarPool::InitializeProgressBars(const float& NewValue, const FPlayerHealthBarPoolInfo& HealthBarPoolInfo)
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

		UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBox->AddChildToHorizontalBox(HealthBar);
		if (HorizontalBoxSlot)
		{
			HorizontalBoxSlot->SetSize(SlateChildSize);
		}
	}
}

void UPlayerHealthBarPool::UpdateProgressBars(const float& NewValue, const FPlayerHealthBarPoolInfo& HealthBarInfo)
{
	const int32 NumBarsToFill = FMath::FloorToInt(NewValue / HealthPerBar);
	const float Remainder = FMath::Fmod(NewValue, HealthPerBar);
	const int32 NumChildren = HealthBarInfo.HorizontalBox->GetChildrenCount();

	for (int i = 0; i < NumChildren; ++i)
	{
		UHealthBar* HealthBar = Cast<UHealthBar>(HealthBarInfo.HorizontalBox->GetChildAt(i));
		if (HealthBar)
		{
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
}

void UPlayerHealthBarPool::UpdateMaxHealthBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_MaxHealth];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	if (SavedHealth > 0.f)
	{
		UpdateProgressBars(SavedHealth, HealthBarPoolInfo); 

		SavedHealth = 0.f;
	}

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UPlayerHealthBarPool::UpdateMaxArmorBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_MaxArmor];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	if (SavedArmor > 0.f)
	{
		UpdateProgressBars(SavedArmor, HealthBarPoolInfo);

		SavedArmor = 0.f;
	}

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UPlayerHealthBarPool::UpdateMaxShieldBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_MaxShield];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	if (SavedShield > 0.f)
	{
		UpdateProgressBars(SavedShield, HealthBarPoolInfo);

		SavedShield = 0.f;
	}

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UPlayerHealthBarPool::UpdateHealthBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_Health];
	if (HealthBarPoolInfo.HorizontalBox->GetChildrenCount() == 0)
	{
		SavedHealth = NewValue;
		return;
	}

	UpdateProgressBars(NewValue, HealthBarPoolInfo);
}

void UPlayerHealthBarPool::UpdateArmorBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_Armor];
	if (HealthBarPoolInfo.HorizontalBox->GetChildrenCount() == 0)
	{
		SavedArmor = NewValue; 
		return;
	}

	UpdateProgressBars(NewValue, HealthBarPoolInfo);
}

void UPlayerHealthBarPool::UpdateShieldBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_Shield];
	if (HealthBarPoolInfo.HorizontalBox->GetChildrenCount() == 0)
	{
		SavedShield = NewValue; 
		return;
	}

	UpdateProgressBars(NewValue, HealthBarPoolInfo);
}

void UPlayerHealthBarPool::UpdateTempArmorBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_TempArmor];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UPlayerHealthBarPool::UpdateTempShieldBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_TempShield];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}

void UPlayerHealthBarPool::UpdateOverHealthBars(float NewValue)
{
	const FPlayerHealthBarPoolInfo& HealthBarPoolInfo = TagsToHealthBarInfos[FOWGameplayTags::Get().Attributes_Defense_OverHealth];

	InitializeProgressBars(NewValue, HealthBarPoolInfo);

	UpdateBorderVisibility();

	DistributeFillSize();
}


void UPlayerHealthBarPool::UpdateBorderVisibility()
{
	for (FPlayerHealthBarPoolInfo& HealthBarInfo : HealthBarInfos)
	{
		UHorizontalBox* HorizontalBox = HealthBarInfo.HorizontalBox;
		UBorder* Border = HealthBarInfo.Border;

		if (HorizontalBox->GetChildrenCount() > 0 && Border)
		{
			Border->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Border->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UPlayerHealthBarPool::DistributeFillSize()
{
	int32 NumAllChildren = 0;

	for (const FPlayerHealthBarPoolInfo& HealthBarInfo : HealthBarInfos)
	{
		UHorizontalBox* HorizontalBox = HealthBarInfo.HorizontalBox;
		if (HorizontalBox)
		{
			NumAllChildren += HorizontalBox->GetChildrenCount();
		}
	}

	FSlateChildSize ChildSize;
	ChildSize.SizeRule = ESlateSizeRule::Fill;

	for (const FPlayerHealthBarPoolInfo& HealthBarInfo : HealthBarInfos)
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

void UPlayerHealthBarPool::ClearHealthBarPool()
{
	if (HorizontalBox_Health)
	{
		HorizontalBox_Health->ClearChildren();
	}

	if (HorizontalBox_Armor)
	{
		HorizontalBox_Armor->ClearChildren();
	}

	if (HorizontalBox_TempArmor)
	{
		HorizontalBox_TempArmor->ClearChildren();
	}

	if (HorizontalBox_Shield)
	{
		HorizontalBox_Shield->ClearChildren();
	}

	if (HorizontalBox_TempShield)
	{
		HorizontalBox_TempShield->ClearChildren();
	}

	if (HorizontalBox_OverHealth)
	{
		HorizontalBox_OverHealth->ClearChildren();
	}
}



