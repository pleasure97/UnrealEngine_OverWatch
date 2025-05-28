// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelectionList.h"
#include "UI/Widget/HeroSelection/RoleGroupList.h"
#include "Components/Button.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "OWGameplayTags.h"

void UHeroSelectionList::NativeConstruct()
{
	Super::NativeConstruct(); 

	bHeroSelected = false; 
	SelectedHeroName = EHeroName::None; 

	check(WidgetController); 

	if (WBP_TankGroupList)
	{
		WBP_TankGroupList->SetRoleGroupList(EHeroClass::Tank);
		WBP_TankGroupList->HeroSelectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroSelected); 
		WBP_TankGroupList->HeroUnselectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroUnselected); 
	}
	if (WBP_DamageGroupList)
	{
		WBP_DamageGroupList->SetRoleGroupList(EHeroClass::Damage);
		WBP_DamageGroupList->HeroSelectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroSelected);
		WBP_DamageGroupList->HeroUnselectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroUnselected);
	}
	if (WBP_SupportGroupList)
	{
		WBP_SupportGroupList->SetRoleGroupList(EHeroClass::Support);
		WBP_SupportGroupList->HeroSelectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroSelected);
		WBP_SupportGroupList->HeroUnselectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroUnselected);
	}

	if (Button_HeroSelect)
	{
		Button_HeroSelect->OnPressed.AddDynamic(this, &UHeroSelectionList::OnHeroSelectButtonPressed); 
	}
}

void UHeroSelectionList::OnHeroSelectButtonPressed()
{
	if (bHeroSelected)
	{
		// TODO - Get Player State and Change Hero Name
		
	}
	else
	{
		FGameplayTag ChangeHeroTag = FOWGameplayTags::Get().Event_ChangeHero;
		UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this);
		TSubclassOf<UGameplayAbility> ChangeHeroAbilityClass = nullptr;

		if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
		{
			for (TSubclassOf<UGameplayAbility>& AbilityClass : HeroInfo->CommonAbilities)
			{
				UGameplayAbility* Ability = AbilityClass->GetDefaultObject<UGameplayAbility>();
				if (Ability->AbilityTags.HasTag(ChangeHeroTag))
				{
					ChangeHeroAbilityClass = AbilityClass;
					break;
				}
			}

			FGameplayEventData EventData;
			EventData.EventTag = ChangeHeroTag;
			EventData.EventMagnitude = static_cast<float>(SelectedHeroName);

			OverlayWidgetController->AbilitySystemComponent->HandleGameplayEvent(ChangeHeroTag, &EventData);
		}
	}
}

void UHeroSelectionList::OnHeroSelected(EHeroName HeroName)
{
	// TODO - Get Player State and Change Hero Name

	bHeroSelected = true;

	SelectedHeroName = HeroName;
}

void UHeroSelectionList::OnHeroUnselected()
{
	// TODO - Get Player State and Change Hero Name

	bHeroSelected = false; 

	SelectedHeroName = EHeroName::None; 
}


