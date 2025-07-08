// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/RoleGroupList.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Components/Image.h"
#include "UI/Widget/HeroSelection/HeroSelection.h"
#include "Components/HorizontalBox.h"

void URoleGroupList::SetRoleGroupList(EHeroClass InHeroClass)
{
	HeroClass = InHeroClass; 

	// Get Hero Info Using Custom Ability System Library 
	UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this); 
	if (HeroInfo)
	{
		// Set Image Role Group 
		const UTexture2D* HeroClassIcon = HeroInfo->CommonClassInformation[HeroClass].ClassIcon;
		if (Image_RoleGroup)
		{
			Image_RoleGroup->SetBrushFromTexture(const_cast<UTexture2D*>(HeroClassIcon), true);
		}

		// Iterate Hero Information and Find Hero that Fits Defined Role Group 
		for (const TPair<EHeroName, FOWHeroInfo>& Pair : HeroInfo->HeroInformation)
		{
			const FOWHeroInfo& Info = Pair.Value; 
			if (Info.HeroClass == HeroClass)
			{
				UHeroSelection* HeroSelectionWidget = CreateWidget<UHeroSelection>(this, HeroSelectionWidgetClass); 
				if (HeroSelectionWidget)
				{
					// Set Information and Bind Selection Delegate to Callback 
					HeroSelectionWidget->SetHeroInfo(Pair.Key);
					HeroSelectionWidget->OnHeroSelected.AddDynamic(this, &URoleGroupList::OnHeroSelected); 

					HorizontalBox_HeroList->AddChild(HeroSelectionWidget); 
				}
			}
		}
	}
}

void URoleGroupList::OnHeroSelected(EHeroName HeroName)
{
	// Broadcast Hero Name 
	HeroSelectedSignature.Broadcast(HeroName); 
}



