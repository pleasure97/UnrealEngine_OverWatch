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
		OverlayWidgetController->OnNumCurrentBulletsChanged.AddDynamic(this, &UPlayerWeaponStatus::UpdateNumMaxBullets);

	}
}

void UPlayerWeaponStatus::UpdateWeaponStatus(const FOWAbilityInfo& Info)
{
	if (!Info.InputTag.MatchesTagExact(FOWGameplayTags::Get().InputTag_LMB)) { return; }

	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(const_cast<UTexture2D*>(Info.Icon.Get()));
	SlateBrush.SetImageSize(FVector2D(100.f, 100.f));
	Image_Weapon->SetBrush(SlateBrush); 
}

void UPlayerWeaponStatus::UpdateNumCurrentBullets(const FGameplayTag& Tag, float NewValue)
{
	TextBlock_NumCurrentBullets->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(NewValue)));
}

void UPlayerWeaponStatus::UpdateNumMaxBullets(const FGameplayTag& Tag, float NewValue)
{
	TextBlock_NumMaxBullets->SetText(FText::AsNumber(UKismetMathLibrary::FTrunc(NewValue))); 
}
