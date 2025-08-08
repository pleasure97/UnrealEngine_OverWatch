// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerWeaponStatus.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "OWGameplayTags.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerWeaponStatus::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		SetWidgetController(OverlayWidgetController); 
		OverlayWidgetController->AbilityInfoDelegate.AddDynamic(this, &UPlayerWeaponStatus::UpdateWeaponStatus); 
		OverlayWidgetController->OnNumCurrentBulletsChanged.AddDynamic(this, &UPlayerWeaponStatus::UpdateNumCurrentBullets);
		OverlayWidgetController->OnNumMaxBulletsChanged.AddDynamic(this, &UPlayerWeaponStatus::UpdateNumMaxBullets);

	}
}

void UPlayerWeaponStatus::UpdateWeaponStatus(const FOWAbilityInfo& Info)
{
	if (!Info.InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_LMB)) { return; }

	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(Info.Icon.Get()));
	Image_Weapon->SetBrush(SlateBrush); 
}

void UPlayerWeaponStatus::UpdateNumCurrentBullets(float NewValue)
{
	if (TextBlock_NumCurrentBullets)
	{
		TextBlock_NumCurrentBullets->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(NewValue))); 
	}
}

void UPlayerWeaponStatus::UpdateNumMaxBullets(float NewValue)
{
	if (NewValue < 0)
	{
		if (TextBlock_NumCurrentBullets)
		{
			TextBlock_NumCurrentBullets->SetText(InfiniteText); 
		}
		if (TextBlock_Slash)
		{
			TextBlock_Slash->SetVisibility(ESlateVisibility::Collapsed); 
		}
		if (TextBlock_NumMaxBullets)
		{
			TextBlock_NumMaxBullets->SetVisibility(ESlateVisibility::Collapsed); 
		}
		return;
	}

	if (TextBlock_NumMaxBullets)
	{
		TextBlock_NumMaxBullets->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(NewValue)));
	}
}
