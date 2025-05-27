// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HeroSelection.generated.h"

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
	TObjectPtr<UButton> Button_HeroSelection; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroPortrait2D;	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HeroName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_HeroName; 
};
