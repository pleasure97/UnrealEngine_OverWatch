// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "Message/OWMessageTypes.h"
#include "AssaultOverlay.generated.h"

class UMatchScoringComponent;
class UAssaultProgress; 
class UAssaultTimer; 
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
	TObjectPtr<UAssaultTimer> Ally_AssaultTimer; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultTimer> Enemy_AssaultTimer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultScore> Ally_AssaultScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssaultScore> Enemy_AssaultScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Mission; 

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

protected:
	virtual void NativeConstruct() override;
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

	UFUNCTION()
	void OnAssaultPointRegistered(AAssaultPoint* AssaultPoint); 

	UFUNCTION()
	void OnTeamChanged(bool bTeamSet, int32 TeamID); 

	UFUNCTION()
	void OnSwitchInningEnded(const FGameplayTag& PhaseTag); 

	UFUNCTION()
	void OnFirstTeamOffenseStarted(const FGameplayTag& PhaseTag);

	UFUNCTION()
	void OnSecondTeamOffenseStarted(const FGameplayTag& PhaseTag);

	UFUNCTION()
	void OnNumAttackersChanged(int32 NewNumAttackers); 

	UFUNCTION()
	void OnNumDefendersChanged(int32 NewNumDefenders);

	UFUNCTION()
	void OnOccupationProgressChanged(float NewOccupationProgress);

	UFUNCTION()
	void OnOccupationStateChanged(EOccupationState NewOccupationState);
};
