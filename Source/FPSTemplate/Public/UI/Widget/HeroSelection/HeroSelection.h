// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "HeroSelection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSelected, EHeroName, HeroName); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeroUnselected); 

namespace HeroSelectionButtonColors
{
	constexpr FLinearColor Orange(1.f, 0.295461f, 0.f, 1.f);
	constexpr FLinearColor Gray(0.1f, 0.1f, 0.1f, 1.f);
	constexpr FLinearColor White(0.9f, 0.9f, 0.9f, 1.f); 
}

class UOverlay; 
class UButton; 
class UImage; 
class UBorder; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroSelection : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_HeroSelection;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_HeroSelection; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HeroSelection; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroPortrait2D;	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HeroName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_HeroName; 
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroSelected; 

	UPROPERTY(BlueprintAssignable)
	FOnHeroSelected OnHeroSelected; 

	UPROPERTY(BlueprintAssignable)
	FOnHeroUnselected OnHeroUnselected;

	void SetHeroInfo(EHeroName InHeroName);

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UPROPERTY()
	EHeroName HeroName;

	UFUNCTION()
	void OnHeroSelectionButtonClicked(); 

	UFUNCTION()
	void OnHeroSelectionButtonHovered();

	UFUNCTION()
	void OnHeroSelectionButtonUnhovered();

	UFUNCTION()
	void OnHeroSelectionButtonReleased();
};
