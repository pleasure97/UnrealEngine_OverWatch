// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
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
	TObjectPtr<URoleGroupList> WBP_HealGroupList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_HeroSelectionList; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_BeforeHeroSelect; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_AfterHeroSelect;
};
