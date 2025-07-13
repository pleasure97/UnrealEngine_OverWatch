// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "HeroSelectionOverlay.generated.h"

class UImage; 
class UTextBlock; 
class UOverlay; 
class UTeamMemberInfoList; 
class UHeroSelectionList; 
class UTimerWidget;
class AOWPlayerState; 

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
	TObjectPtr<UTimerWidget> WBP_FirstHeroSelectionTimer; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTimerWidget> WBP_SecondHeroSelectionTimer; 

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlueColor = FLinearColor(0.041667f, 0.6407f, 1.f, 1.f); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RedColor = FLinearColor(0.609375f, 0.f, 0.f, 0.8f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttackMissionText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DefendMissionText; 

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override; 

private:
	UFUNCTION()
	void OnHeroNameChanged(AOWPlayerState* PlayerState, EHeroName HeroName);
};
