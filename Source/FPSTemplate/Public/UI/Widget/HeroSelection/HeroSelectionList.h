// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "HeroSelectionList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeroButtonClicked, EHeroName, HeroID, UHeroSelection*, ClickedButton);

class UHorizontalBox; 
class UButton; 
class URoleGroupList;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroSelectionList : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_HeroSelectionList; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_HeroSelect; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URoleGroupList> RoleGroupListClass; 

	void ChooseNewHero(); 

	void InitializeHeroSelectionList();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override; 

private:
	UFUNCTION()
	void OnHeroSelectButtonClicked();

	UFUNCTION()
	void OnHeroSelected(EHeroName HeroName); 

	UFUNCTION()
	void OnHeroUnselected(); 

	UPROPERTY()
	bool bHeroSelected = false; 

	UPROPERTY()
	EHeroName SelectedHeroName; 
};
