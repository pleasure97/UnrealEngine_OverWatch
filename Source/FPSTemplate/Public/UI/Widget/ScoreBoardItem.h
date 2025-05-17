// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OWUserWidget.h"
#include "ScoreBoardItem.generated.h"

class UBorder; 
class UImage; 
class UTextBlock; 
class UMaterialInstanceDynamic; 
class UThrobber; 

namespace ScoreBoardColors
{
	constexpr FLinearColor OriginalBlue(0.041667f, 0.6407f, 1.f, 1.f);
	constexpr FLinearColor OpacityBlue(0.041667f, 0.6407f, 1.f, 0.8f);
	constexpr FLinearColor DarkBlue(0.004774f, 0.073414f, 0.114583f, 0.8f);
	constexpr FLinearColor OriginalRed(0.520833f, 0.005426f, 0.032740f, 1.f);
	constexpr FLinearColor OpacityRed(0.520833f, 0.005426f, 0.032740f, 0.8f);
	constexpr FLinearColor DarkRed(0.171875f, 0.001791f, 0.010804f, 0.8f);
}

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API UScoreBoardItem : public UOWUserWidget
{
	GENERATED_BODY()
	
public:
	/* Role Group */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_RoleGroup; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_RoleGroup;

	/* Hero Info */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HeroInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_HeroPortrait2D;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UThrobber> Throbber_WaitingPlayer;

	/* Death Duration */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_DeathDuration;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_DeathDuration;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MID_DeathDuration; 

	/* Ultimate Gauge */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_UltimateGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_UltimateGauge; 

	/* Player Name */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerName;

	/* Kill */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_KillInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumKills;

	/* Death */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_DeathInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumDeaths;

	/* Assist */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_AssistInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NumAssists;

	/* Damage */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_DamageInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_DamageAmount;

	/* Heal */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HealInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_HealAmount;

	/* Tank */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_TankInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_TankAmount;

protected:
	virtual void NativeConstruct() override; 
};
