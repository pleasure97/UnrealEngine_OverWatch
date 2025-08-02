// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HeroSelection/TeamMemberInfo.h"
#include "Player/OWPlayerState.h"
#include "AbilitySystem/OWAbilitySystemLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Throbber.h"

void UTeamMemberInfo::NativeConstruct()
{
	Super::NativeConstruct(); 

	// Set the Visibility of All Widgets except Throbber to Collapsed
	if (Image_HeroPortrait3D)
	{
		Image_HeroPortrait3D->SetVisibility(ESlateVisibility::Collapsed); 
	}

	if (Image_HeroSelected)
	{
		Image_HeroSelected->SetVisibility(ESlateVisibility::Collapsed); 
	}

	if (Image_RoleGroup)
	{
		Image_RoleGroup->SetVisibility(ESlateVisibility::Collapsed); 
	}

	if (TextBlock_Nickname)
	{
		TextBlock_Nickname->SetVisibility(ESlateVisibility::Collapsed); 
	}

	UpdateNickName(); 
}

void UTeamMemberInfo::RefreshTeamMemberInfo(AOWPlayerState* InOwnerPlayerState)
{
	OwnerPlayerState = InOwnerPlayerState;

	UpdateNickName();
}

void UTeamMemberInfo::UpdateNickName()
{
	// Check if Owner Player State is not nullptr 
	if (OwnerPlayerState != nullptr)
	{
		if (TextBlock_Nickname)
		{
			// Clamp Nickname
			FString NickNameString = OwnerPlayerState->GetPlayerName(); 
			if (NickNameString.Len() > NicknameMaxLength)
			{
				NickNameString = NickNameString.Right(NicknameMaxLength); 
			}
			// Set Text and Visibility of TextBlock
			TextBlock_Nickname->SetText(FText::FromString(NickNameString));
			TextBlock_Nickname->SetVisibility(ESlateVisibility::Visible); 
		}
	}
	else
	{
		if (TextBlock_Nickname)
		{
			// Set Visibility of TextBlock
			TextBlock_Nickname->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}
}

AOWPlayerState* UTeamMemberInfo::GetOWPlayerState() const
{
	return OwnerPlayerState; 
}

void UTeamMemberInfo::HandleHeroNameChanged(AOWPlayerState* OWPlayerState, EHeroName HeroName)
{
	// Check if Hero Name is None
	// If there is No Hero Name, Client May be Disconnected or Deselectg Hero 
	if (!OWPlayerState || HeroName == EHeroName::None)
	{
		if (Throbber_WaitingTeamMember)
		{
			Throbber_WaitingTeamMember->SetVisibility(ESlateVisibility::Visible); 
		}
		if (Image_HeroSelected)
		{
			Image_HeroSelected->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (Image_HeroPortrait3D)
		{
			Image_HeroPortrait3D->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (Image_RoleGroup)
		{
			Image_RoleGroup->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		UHeroInfo* HeroInfo = UOWAbilitySystemLibrary::GetHeroInfo(this);
		FOWHeroInfo& IndividualHeroInfo = HeroInfo->HeroInformation[HeroName];

		if (Throbber_WaitingTeamMember)
		{
			Throbber_WaitingTeamMember->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (Image_HeroSelected)
		{
			Image_HeroSelected->SetVisibility(ESlateVisibility::Visible);
		}
		if (Image_HeroPortrait3D)
		{
			Image_HeroPortrait3D->SetVisibility(ESlateVisibility::Visible);
			Image_HeroPortrait3D->SetBrushFromTexture(const_cast<UTexture2D*>(IndividualHeroInfo.HeroPortrait3D.Get()), true);
		}
		if (Image_RoleGroup)
		{
			Image_RoleGroup->SetVisibility(ESlateVisibility::Visible);
			const UTexture2D* RoleGroupImage = HeroInfo->CommonClassInformation[IndividualHeroInfo.HeroClass].ClassIcon;
			Image_RoleGroup->SetBrushFromTexture(const_cast<UTexture2D*>(RoleGroupImage), true);
		}
	}
}


