// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "Message/OWMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "AssaultOverlay.generated.h"

class UMatchScoringComponent;
class UAssaultProgress; 
class UAssaultScoreDetails; 
class UAssaultScore; 
class AAssaultPoint; 
class UTextBlock; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAssaultOverlay : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Subwidgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultProgress> WBP_AssaultProgress; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultScoreDetails> Ally_AssaultScoreDetails; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultScoreDetails> Enemy_AssaultScoreDetails;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultScore> Ally_AssaultScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultScore> Enemy_AssaultScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Mission; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_RoundRemainingTime; 

	/* Settings */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlueTeamColor; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RedTeamColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor NeutralColor; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PrepareOffenseText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PrepareDefenseText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText OffenseText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DefenseText; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RoundEndText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override; 

	void UpdateOffensePreparation();
	void UpdateDefensePreparation(); 

private:
	UPROPERTY()
	TObjectPtr<UMatchScoringComponent> MatchScoringComponent;

	UPROPERTY()
	int32 OwnerTeamID = -1;

	UPROPERTY()
	int32 CurrentOffenseTeamID = -1; 

	UPROPERTY()
	FGameplayTag CurrentGamePhaseTag; 

	UFUNCTION()
	void OnAssaultPointRegistered(AAssaultPoint* AssaultPoint); 

	UFUNCTION()
	void OnTeamChanged(bool bTeamSet, int32 TeamID); 

	/* Game Phase */
	UFUNCTION()
	void ProcessPhaseRemainingTime(const FGameplayTag& PhaseTag, const float RemainingTime);

	/* Occupation */
	UFUNCTION()
	void OnNumAttackersChanged(int32 NewNumAttackers); 

	UFUNCTION()
	void OnNumDefendersChanged(int32 NewNumDefenders);

	UFUNCTION()
	void OnOccupationProgressChanged(float NewOccupationProgress);

	UFUNCTION()
	void OnOccupationStateChanged(EOccupationState NewOccupationState);

	UFUNCTION()
	void HandleCountdownTime(FGameplayTag Channel, const FOWVerbMessage& Payload);

	FText ConvertTimeText(float RemainingCountdownTime); 

	bool bAssaultPointRegistered = false; 

	float TargetProgress = 0.f; 

	float DisplayedProgress = 0.f; 

	bool bMissionUpdated = false; 

	FGameplayMessageListenerHandle FirstHeroSelectionListenerHandle;
	FGameplayMessageListenerHandle FirstMatchPreparationListenerHandle;
	FGameplayMessageListenerHandle FirstTeamOffenseListenerHandle;
	FGameplayMessageListenerHandle SwitchInningListenerHandle;
	FGameplayMessageListenerHandle SecondHeroSelectionListenerHandle;
	FGameplayMessageListenerHandle SecondMatchPreparationListenerHandle;
	FGameplayMessageListenerHandle SecondTeamOffenseListenerHandle;
	FGameplayMessageListenerHandle PostMatchListenerHandle; 
};
