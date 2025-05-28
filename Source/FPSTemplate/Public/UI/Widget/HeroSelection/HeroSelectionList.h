// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "HeroSelectionList.generated.h"

class URoleGroupList; 
class UWidgetSwitcher; 
class UButton; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroSelectionList : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoleGroupList> WBP_TankGroupList; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoleGroupList> WBP_DamageGroupList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URoleGroupList> WBP_SupportGroupList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_HeroSelect; 

protected:
	virtual void NativeConstruct() override; 

private:
	UFUNCTION()
	void OnHeroSelectButtonPressed();

	UFUNCTION()
	void OnHeroSelected(EHeroName HeroName); 

	UFUNCTION()
	void OnHeroUnselected(); 

	UPROPERTY()
	bool bHeroSelected = false; 

	UPROPERTY()
	EHeroName SelectedHeroName; 
};
