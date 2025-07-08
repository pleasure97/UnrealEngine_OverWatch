// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "RoleGroupList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroSelectedSignature, EHeroName, HeroName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroUnselectedSignature);

class UImage; 
class UHorizontalBox; 
class UHeroSelection; 
class UTexture2D; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API URoleGroupList : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_RoleGroup; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_HeroList; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UHeroSelection> HeroSelectionWidgetClass; 

	void SetRoleGroupList(EHeroClass InHeroClass); 

	UPROPERTY(BlueprintAssignable)
	FHeroSelectedSignature HeroSelectedSignature;

	UPROPERTY(BlueprintAssignable)
	FHeroUnselectedSignature HeroUnselectedSignature;

private:
	UPROPERTY()
	EHeroClass HeroClass; 

	UFUNCTION()
	void OnHeroSelected(EHeroName HeroName); 
};
