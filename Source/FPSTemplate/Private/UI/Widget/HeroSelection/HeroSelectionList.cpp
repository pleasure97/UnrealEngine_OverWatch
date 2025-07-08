// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelectionList.h"
#include "UI/Widget/HeroSelection/RoleGroupList.h"
#include "UI/Widget/HeroSelection/HeroSelection.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "OWGameplayTags.h"
#include "Player/OWPlayerController.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

void UHeroSelectionList::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Initialize that Hero is not Selected. 
	bHeroSelected = false; 
	SelectedHeroName = EHeroName::None; 
}

void UHeroSelectionList::NativeDestruct()
{
	// Remove All Bindings of Role Group List Delegates
	if (HorizontalBox_HeroSelectionList)
	{
		for (UWidget* ChildWidget : HorizontalBox_HeroSelectionList->GetAllChildren())
		{
			if (URoleGroupList* RoleGroupListWidget = Cast<URoleGroupList>(ChildWidget))
			{
				RoleGroupListWidget->HeroSelectedSignature.RemoveAll(this);
				RoleGroupListWidget->HeroUnselectedSignature.RemoveAll(this); 
			}
		}
	}
	
	// Remove Hero Select Button Binding 
	if (Button_HeroSelect)
	{
		Button_HeroSelect->OnClicked.RemoveAll(this); 
	}

	Super::NativeDestruct(); 
}

void UHeroSelectionList::InitializeHeroSelectionList()
{
	// Get Hero Info using Custom Ability System Library 
	UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this); 
	if (HeroInfo)
	{
		// Iterate All Hero Classes from Hero Info Data Asset 
		for (EHeroClass HeroClass : HeroInfo->GetAllHeroClasses())
		{
			// Create New Role Group List Widget 
			URoleGroupList* NewRoleGroupList = CreateWidget<URoleGroupList>(this, RoleGroupListClass);
			if (NewRoleGroupList)
			{
				// Set Role Group List's Hero Class
				NewRoleGroupList->SetRoleGroupList(HeroClass);
				// Bind Role Group List's Delegates 
				NewRoleGroupList->HeroSelectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroSelected);
				NewRoleGroupList->HeroUnselectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroUnselected);
				if (HorizontalBox_HeroSelectionList)
				{
					// Add New Role Group List to Horizontal Box 
					if (UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBox_HeroSelectionList->AddChildToHorizontalBox(NewRoleGroupList))
					{
						// Fill New Role Group List 
						FSlateChildSize SlateChildSize; 
						SlateChildSize.SizeRule = ESlateSizeRule::Fill; 
						SlateChildSize.Value = 1.f; 
						HorizontalBoxSlot->SetSize(SlateChildSize);
					}
				}
			}
		}
	}
	
	// Add Hero Select Button's Binding 
	if (Button_HeroSelect)
	{
		Button_HeroSelect->OnClicked.AddDynamic(this, &UHeroSelectionList::OnHeroSelectButtonClicked);
	}
}

void UHeroSelectionList::OnHeroSelectButtonClicked()
{
	if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetOwningPlayer()))
	{
		OWPlayerController->ServerChooseHero(SelectedHeroName); 
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

void UHeroSelectionList::ChooseNewHero()
{

}

