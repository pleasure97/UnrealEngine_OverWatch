// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "HeroSelectionList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeroSelectButtonClicked, EHeroName, HeroName, bool, bConfirmed);

class UHorizontalBox; 
class UButton; 
class UTextBlock; 
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_HeroSelect; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URoleGroupList> RoleGroupListClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText HeroSelectText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText HeroChangeText; 

	FOnHeroSelectButtonClicked HeroSelectButtonDelegate; 

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
	void OnHeroSelectionInitialized(EHeroName HeroName, UHeroSelection* HeroSelection);

	void MakeHeroSelectionButtonsInvisible(bool bInvisible); 

	UPROPERTY()
	EHeroName SelectedHeroName; 

	TMap<EHeroName, UHeroSelection*> HeroSelectionButtonMap; 
	
	TMap<EHeroClass, URoleGroupList*> RoleGroupListMap; 

	bool bHeroAlreadyChosen = false; 
	bool bHeroSelectConfirmed = false; 
};
