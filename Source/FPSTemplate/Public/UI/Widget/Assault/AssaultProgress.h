// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "GameplayTagContainer.h"
#include "Message/OWMessageTypes.h"
#include "AssaultProgress.generated.h"

class UImage; 
class UHorizontalBox; 
class UBorder; 
class UTextBlock; 
class AAssaultPoint; 
class UMaterialInstanceDynamic; 
class UWidgetAnimation; 

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UAssaultProgress : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Contesting */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Contesting;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Contesting;

	/* Assault Point Default Information */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_AssaultProgress;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_AssaultRhombus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> BlueAssaultRhombus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> RedAssaultRhombus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BlueTeamColor = FLinearColor(0.041667f, 0.6407f, 1.f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RedTeamColor = FLinearColor(0.65625f, 0.f, 0.f, 1.f);

	/* Assault Point Occupation */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_NumOccupied;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_NumOccupiedAttackers; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumOccupiedAttackers;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_NumOccupiedDefenders;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumOccupiedDefenders;

	void UpdateNumAttackers(int32 NumAttackers);
	void UpdateNumDefenders(int32 NumDefenders);
	void UpdateOccupationProgress(float OccupationProgress);
	void UpdateOccupationState(EOccupationState OccupationState);
	void PlayContestingAnimation();
	void StopContestingAnimation();
	void UpdateProgressDesign(int32 OffenseTeamID); 

protected:
	virtual void NativeConstruct() override;
	void HideAssaultWidgets();


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; 
private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> AssaultProgressMID;

	FGameplayTag CurrentGamePhase; 

	int32 CurrentNumAttackers = -1;
	
	int32 OwnerTeamID = -1; 
};
