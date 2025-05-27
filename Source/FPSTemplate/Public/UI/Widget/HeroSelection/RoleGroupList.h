// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "RoleGroupList.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EHeroClass, UTexture2D*> RoleGroupMap; 
};
