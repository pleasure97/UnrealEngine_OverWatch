// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/TeamMemberInfo.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Throbber.h"

void UTeamMemberInfo::HandleHeroNameChanged(AOWPlayerState* OWPlayerState, EHeroName HeroName)
{
	if (!bInfoSet)
	{
		if (TextBlock_Nickname)
		{
			TextBlock_Nickname->SetText(FText::FromString(OWPlayerState->GetPlayerName())); 
		}
		if (Throbber_WaitingTeamMember)
		{
			Throbber_WaitingTeamMember->SetVisibility(ESlateVisibility::Collapsed); 
		}
		bInfoSet = true; 
	}

	UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this); 
	FOWHeroInfo& IndividualHeroInfo = HeroInfo->HeroInformation[HeroName]; 
	if (Image_HeroPortrait3D)
	{
		Image_HeroPortrait3D->SetBrushFromTexture(const_cast<UTexture2D*>(IndividualHeroInfo.HeroPortrait3D.Get()), true);
	}
	if (Image_RoleGroup)
	{
		const UTexture2D* RoleGroupImage = HeroInfo->CommonClassInformation[IndividualHeroInfo.HeroClass].ClassIcon; 
		Image_RoleGroup->SetBrushFromTexture(const_cast<UTexture2D*>(RoleGroupImage), true); 
	}
}
