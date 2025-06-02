// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "HeroSelectionOverlay.generated.h"

class UImage; 
class UTextBlock; 
class UOverlay; 
class UTeamMemberInfoList; 
class UHeroSelectionList; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UHeroSelectionOverlay : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroSelectionOverlay; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MainMission; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MissionDescription; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PreparationTime; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_HeroName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Mission; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MapName; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_WaitingForBattle; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTeamMemberInfoList> WBP_TeamMemberInfoList; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHeroSelectionList> WBP_HeroSelectionList; 
};
