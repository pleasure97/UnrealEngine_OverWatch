// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "AbilitySystem/Data/HeroInfo.h"
#include "Message/OWMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "HeroSelectionOverlay.generated.h"

class UImage; 
class UTextBlock; 
class UOverlay; 
class UTeamMemberInfoList; 
class UHeroSelectionList; 
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
	TObjectPtr<UTextBlock> TextBlock_RemainingTime; 

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

	/* Design and Localization Settings */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlueColor = FLinearColor(0.041667f, 0.6407f, 1.f, 1.f); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RedColor = FLinearColor(0.609375f, 0.f, 0.f, 0.8f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText WaitingForTeamConstructionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttackMissionText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DefendMissionText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttackPreparationMissionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DefendPreparationMissionText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override; 

private:
	UFUNCTION()
	void OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID); 

	UFUNCTION()
	void OnHeroSelectButtonClicked(EHeroName HeroName, bool bConfirmed);

	/* Game Phase*/
	UFUNCTION(BlueprintCallable)
	void ProcessPhaseRemainingTime(const FGameplayTag& PhaseTag, const float RemainingTime); 

	FText ConvertTimeText(float RemainingCountdownTime);

	UFUNCTION()
	void HandleCountdownTime(FGameplayTag Channel, const FOWVerbMessage& Payload); 

	UPROPERTY()
	FGameplayTag CurrentGamePhaseTag; 

	UPROPERTY()
	EHeroName SelectedHeroName; 

	UPROPERTY()
	int32 OwnerTeamID = -1; 

	UPROPERTY()
	TObjectPtr<AOWPlayerState> OwnerPlayerState; 

	bool bMissionDescriptionUpdated = false; 

	FGameplayMessageListenerHandle FirstHeroSelectionListenerHandle;
	FGameplayMessageListenerHandle FirstMatchPreparationListenerHandle;
	FGameplayMessageListenerHandle FirstTeamOffenseListenerHandle;
	FGameplayMessageListenerHandle SecondHeroSelectionListenerHandle;
	FGameplayMessageListenerHandle SecondMatchPreparationListenerHandle;
	FGameplayMessageListenerHandle SecondTeamOffenseListenerHandle;
	FGameplayMessageListenerHandle PostMatchListenerHandle;
};
