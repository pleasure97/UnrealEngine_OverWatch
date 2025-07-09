// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "HeroSelection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSelected, EHeroName, HeroName); 

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
	/* Widgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_HeroSelection;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_HeroSelection; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HeroSelection; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroPortrait3D;	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HeroName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_HeroName; 
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroSelected; 

	/* Color Settings */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor OrangeColor = FLinearColor(1.f, 0.295461f, 0.f, 1.f); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor GrayColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor WhiteColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(BlueprintAssignable)
	FOnHeroSelected OnHeroSelected; 

	void SetHeroInfo(EHeroName InHeroName);

	void UpdateUnclicked();

	EHeroName GetHeroName() const; 

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

	bool bClicked = false;
};
