// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/HeroSelectionList.h"
#include "UI/Widget/HeroSelection/RoleGroupList.h"
#include "UI/Widget/HeroSelection/HeroSelection.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "OWGameplayTags.h"
#include "Player/OWPlayerController.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"

void UHeroSelectionList::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Initialize that Hero is not Selected. 
	SelectedHeroName = EHeroName::None; 
}

void UHeroSelectionList::NativeDestruct()
{
	// Remove All Bindings of Role Group List Delegates
	if (HorizontalBox_HeroSelectionList)
	{
		for (TPair<EHeroClass, URoleGroupList*>& RoleGroupListPair : RoleGroupListMap)
		{
			if (RoleGroupListPair.Value)
			{
				RoleGroupListPair.Value->OnHeroSelectionInitialized.RemoveAll(this); 
				RoleGroupListPair.Value->HeroSelectedSignature.RemoveAll(this);
			}
		}
	}

	// Remove Hero Select Button Binding 
	if (Button_HeroSelect)
	{
		Button_HeroSelect->OnClicked.RemoveAll(this); 
	}

	RoleGroupListMap.Empty(); 
	HeroSelectionButtonMap.Empty(); 

	Super::NativeDestruct();
}

void UHeroSelectionList::InitializeHeroSelectionList()
{
	UE_LOG(LogTemp, Log, TEXT("UHeroSelectionList::InitializeHeroSelectionList()"));
	// Get Hero Info using Custom Ability System Library
	UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(GetWorld());
	if (!IsValid(HeroInfo))
	{
		UE_LOG(LogTemp, Log, TEXT("Hero Info is Not Valid in UHeroSelectionList::InitializeHeroSelectionList()"));
		return;
	}

	// Iterate All Hero Classes from Hero Info Data Asset 
	for (EHeroClass HeroClass : HeroInfo->GetAllHeroClasses())
	{
		// Create New Role Group List Widget 
		URoleGroupList* NewRoleGroupList = CreateWidget<URoleGroupList>(this, RoleGroupListClass);
		if (!IsValid(NewRoleGroupList))
		{
			UE_LOG(LogTemp, Log, TEXT("Cannot Create RoleGroupList Widget in UHerroSelectionList::InitializeHeroSelectionList()"));
			continue;
		}

		NewRoleGroupList->OnHeroSelectionInitialized.AddDynamic(this, &UHeroSelectionList::OnHeroSelectionInitialized);
		// Set Role Group List's Hero Class
		NewRoleGroupList->SetRoleGroupList(HeroClass);
		// Bind Role Group List's Delegates 
		NewRoleGroupList->HeroSelectedSignature.AddDynamic(this, &UHeroSelectionList::OnHeroSelected);
		RoleGroupListMap.Add(NewRoleGroupList->GetHeroClass(), NewRoleGroupList);
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

	// Add Hero Select Button's Binding 
	if (Button_HeroSelect)
	{
		Button_HeroSelect->OnClicked.AddDynamic(this, &UHeroSelectionList::OnHeroSelectButtonClicked);
	}
}

void UHeroSelectionList::OnHeroSelectButtonClicked()
{
	if (bHeroSelectConfirmed)
	{
		if (TextBlock_HeroSelect)
		{
			TextBlock_HeroSelect->SetText(HeroSelectText);
		}
		HideHeroListButtons(false);
		bHeroSelectConfirmed = false;
		HeroSelectButtonDelegate.Broadcast(EHeroName::None, false);
		return;
	}

	if (!bHeroAlreadyChosen)
	{
		if (AOWPlayerController* OWPlayerController = Cast<AOWPlayerController>(GetOwningPlayer()))
		{
			OWPlayerController->ServerChooseHero(SelectedHeroName);
			if (TextBlock_HeroSelect)
			{
				TextBlock_HeroSelect->SetText(HeroChangeText); 
			}
			HideHeroListButtons(true); 
			bHeroSelectConfirmed = true;
			HeroSelectButtonDelegate.Broadcast(SelectedHeroName, true); 
		}
	}
}

void UHeroSelectionList::OnHeroSelected(EHeroName HeroName)
{
	if (SelectedHeroName == HeroName)
	{
		bHeroAlreadyChosen = true; 
	}
	else
	{
		if (HeroSelectionButtonMap.Find(SelectedHeroName))
		{
			HeroSelectionButtonMap[SelectedHeroName]->UpdateUnclicked();
		}
		SelectedHeroName = HeroName; 
		bHeroAlreadyChosen = false;
	}
}

void UHeroSelectionList::OnHeroSelectionInitialized(EHeroName HeroName, UHeroSelection* HeroSelection)
{
	if ((HeroName != EHeroName::None) && (HeroSelection != nullptr))
	{
		HeroSelectionButtonMap.Add(HeroName, HeroSelection); 
	}
}

void UHeroSelectionList::HideHeroListButtons(bool bHidden)
{
	if (HorizontalBox_HeroSelectionList)
	{
		ESlateVisibility ButtonVisibility = bHidden ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
		HorizontalBox_HeroSelectionList->SetVisibility(ButtonVisibility);
	}
}


